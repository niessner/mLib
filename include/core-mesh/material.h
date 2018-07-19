#ifndef CORE_MESH_MATERIAL_H_
#define CORE_MESH_MATERIAL_H_

namespace ml {

template<class FloatType>
class Material {
public:
	Material() {
		reset();
	}
	Material(Material&& m) {
		swap(*this, m);
	}
	Material(const Material& m) = default;
	Material& operator=(const Material&) = default;

	void operator=(Material&& m) {
		swap(*this, m);
	}

	//! adl swap
	friend void swap(Material& a, Material& b) {
		std::swap(a.m_name, b.m_name);
		std::swap(a.m_ambient, b.m_ambient);
		std::swap(a.m_diffuse, b.m_diffuse);
		std::swap(a.m_specular, b.m_specular);
		std::swap(a.m_shiny, b.m_shiny);
		std::swap(a.m_emission, b.m_emission);

		std::swap(a.m_TextureFilename_Ka, b.m_TextureFilename_Ka);
		std::swap(a.m_TextureFilename_Kd, b.m_TextureFilename_Kd);
		std::swap(a.m_TextureFilename_Ks, b.m_TextureFilename_Ks);
		std::swap(a.m_TextureFilename_Ke, b.m_TextureFilename_Ke);
		//std::swap(a.m_Texture_Ka, b.m_Texture_Ka);
		//std::swap(a.m_Texture_Kd, b.m_Texture_Kd);
		//std::swap(a.m_Texture_Ks, b.m_Texture_Ks);
	}

	static std::vector<Material> loadFromMTL(const std::string& filename) {
		std::vector<Material> res;
		loadFromMTL(filename, res);
		return res;
	}

	static void loadFromMTL(const std::string& filename, std::vector<Material>& res) {
		res.clear();

		std::ifstream in(filename);
		if (!in.is_open()) throw MLIB_EXCEPTION("could not open file " + filename);
		std::string line;

		Material activeMaterial;	bool found = false;
		while (std::getline(in, line)) {
			std::stringstream ss(line);

			std::string token;
			ss >> token;
            if (token == "newmtl") {
				if (!found) {
					found = true;
				} else {
					res.push_back(activeMaterial);
				}
				activeMaterial.reset();
				ss >> activeMaterial.m_name;
			} else if (token == "Ka") {
				ss >> activeMaterial.m_ambient.x >> activeMaterial.m_ambient.y >> activeMaterial.m_ambient.z;
			} else if (token == "Kd") {
				ss >> activeMaterial.m_diffuse.x >> activeMaterial.m_diffuse.y >> activeMaterial.m_diffuse.z;
			} else if (token == "Ks") {
				ss >> activeMaterial.m_specular.x >> activeMaterial.m_specular.y >> activeMaterial.m_specular.z;
			} else if (token == "Ns") {
				ss >> activeMaterial.m_shiny;
			} else if (token == "Ke") {
				ss >> activeMaterial.m_emission;
			} else if (token == "map_Ka") {
				ss >> activeMaterial.m_TextureFilename_Ka;
			} else if (token == "map_Kd") {
				ss >> activeMaterial.m_TextureFilename_Kd;
			} else if (token == "map_Ks") {
				ss >> activeMaterial.m_TextureFilename_Ks;
			} else if (token == "map_Ke") {
				ss >> activeMaterial.m_TextureFilename_Ke;
			} else if (token == "d") {
				// d token not implemented
				//MLIB_WARNING("d token not implemented");
			} else if (token != "" || (token.size() > 0 && token[0] == '#')) {
				MLIB_WARNING("unknown token: " + line);
			}
		}

		if (found) {
			res.push_back(activeMaterial);
		}
		in.close();


	} 

	static void saveToMTL(const std::string& filename, const std::vector<Material>& _mats) {

		std::vector<Material> mats = _mats;
		auto glambda0 = [](const Material& m0, const Material& m1)	{ return m0.m_name < m1.m_name;	};
		auto glambda1 = [](const Material& m0, const Material& m1)	{ return m0.m_name == m1.m_name; };
		std::sort(mats.begin(), mats.end(), glambda0);
		auto last = std::unique(mats.begin(), mats.end(), glambda1);	//collapsing the same materials
		mats.erase(last, mats.end()); 

		std::ofstream out(filename);
		if (!out.is_open()) throw MLIB_EXCEPTION("could not open file " + filename);

		for (const auto& m : mats) {
			out << "newmtl " << m.m_name << "\n";
			out << "Ka " << m.m_ambient.x << " " << m.m_ambient.y << " " << m.m_ambient.z << "\n";
			out << "Kd " << m.m_diffuse.x << " " << m.m_diffuse.y << " " << m.m_diffuse.z << "\n";
			out << "Ks " << m.m_specular.x << " " << m.m_specular.y << " " << m.m_specular.z << "\n";
			out << "Ns " << m.m_shiny << "\n";
			out << "Ke " << m.m_emission << "\n";
			out << "illum 2" << "\n";	//todo check the illum consistencies
			if (m.m_TextureFilename_Ka != "")	out << "map_Ka " << m.m_TextureFilename_Ka << "\n";
			if (m.m_TextureFilename_Kd != "")	out << "map_Kd " << m.m_TextureFilename_Kd << "\n";
			if (m.m_TextureFilename_Ks != "")	out << "map_Ks " << m.m_TextureFilename_Ks << "\n";
			if (m.m_TextureFilename_Ke != "")	out << "map_Ke " << m.m_TextureFilename_Ke << "\n";

			out << "\n";
		}

		out.close();
	}

	void reset() {
		m_name = "";
		m_ambient = vec4<FloatType>(0,0,0,0);
		m_diffuse = vec4<FloatType>(0,0,0,0);
		m_specular = vec4<FloatType>(0,0,0,0);
		m_shiny = 0;
		m_emission = vec4<FloatType>(0, 0, 0, 0);
		m_TextureFilename_Ka = "";
		m_TextureFilename_Kd = "";
		m_TextureFilename_Ks = "";
		m_TextureFilename_Ke = "";
		//m_Texture_Ka.free();
		//m_Texture_Kd.free();
		//m_Texture_Ks.free();
	}

	std::string		m_name;
	vec4<FloatType>	m_ambient;
	vec4<FloatType>	m_diffuse;
	vec4<FloatType>	m_specular;
	FloatType		m_shiny;
	vec4<FloatType> m_emission;

	std::string			m_TextureFilename_Ka;
	std::string			m_TextureFilename_Kd;
	std::string			m_TextureFilename_Ks;
	std::string			m_TextureFilename_Ke;
	//ColorImageR8G8B8A8	m_Texture_Ka;
	//ColorImageR8G8B8A8	m_Texture_Kd;
	//ColorImageR8G8B8A8	m_Texture_Ks;
};

typedef Material<float> Materialf;
typedef Material<double> Materiald;

template<class BinaryDataBuffer, class BinaryDataCompressor, class FloatType>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator<< (BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, const Material<FloatType> &m) {
	s << m.m_name << m.m_ambient << m.m_diffuse << m.m_specular << m.m_shiny << m.m_emission;
	s << m.m_TextureFilename_Ka;
	s << m.m_TextureFilename_Kd;
	s << m.m_TextureFilename_Ks;
	s << m.m_TextureFilename_Ke;
	return s;
}

template<class BinaryDataBuffer, class BinaryDataCompressor, class FloatType>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator>> (BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, Material<FloatType> &m) {
	s >> m.m_name >> m.m_ambient >> m.m_diffuse >> m.m_specular >> m.m_shiny >> m.m_emission;
	s >> m.m_TextureFilename_Ka;
	s >> m.m_TextureFilename_Kd;
	s >> m.m_TextureFilename_Ks;
	s >> m.m_TextureFilename_Ke;
	return s;
}

}

#endif
