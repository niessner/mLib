
#ifndef CORE_MESH_PLYHEADER_H_
#define CORE_MESH_PLYHEADER_H_

namespace ml {

	struct PlyHeader {
		struct PlyPropertyHeader {
			PlyPropertyHeader() {
				byteSize = 0;
			}
			std::string name;
			std::string nameType;
			unsigned int byteSize;
		};
		PlyHeader(std::ifstream& file) {
			m_numVertices = (unsigned int)-1;
			m_numFaces = (unsigned int)-1;
			m_bHasNormals = false;
			m_bHasColors = false;

			read(file);
		}
		PlyHeader() {
			m_numVertices = (unsigned int)-1;
			m_numFaces = (unsigned int)-1;
			m_bHasNormals = false;
			m_bHasColors = false;
		}
		unsigned int m_numVertices;
		unsigned int m_numFaces;
		std::map<std::string, std::vector<PlyPropertyHeader>> m_properties;
		bool m_bBinary;
		bool m_bHasNormals;
		bool m_bHasColors;

		void read(std::ifstream& file) {
			std::string activeElement = "";
			std::string line;
			util::safeGetline(file, line);
			while (line.find("end_header") == std::string::npos) {
				PlyHeaderLine(line, *this, activeElement);
				util::safeGetline(file, line);
			}
		}

		static unsigned int getByteSizeFromNameType(const std::string& nameType) {
			/*
			nameType: double/float/int/uint/char/uchar/short/ushort
			return: number of bytes occupied by this type
			*/
			unsigned int size;

			if (nameType == "double") size = 8;
			else if (nameType == "float" || nameType == "int" || nameType == "uint") size = 4;
			else if (nameType == "ushort" || nameType == "short") size = 2;
			else if (nameType == "uchar" || nameType == "char") size = 1;
			else {
				throw MLIB_EXCEPTION("unkown data type");
			}
			return size;
		}

		static void PlyHeaderLine(const std::string& line, PlyHeader& header, std::string& activeElement) {

			std::stringstream ss(line);
			std::string currWord;
			ss >> currWord;


			if (currWord == "element") {
				ss >> currWord;
				activeElement = currWord;
				if (currWord == "vertex") {
					ss >> header.m_numVertices;
				}
				else if (currWord == "face") {
					ss >> header.m_numFaces;
				}
			}
			else if (currWord == "format") {
				ss >> currWord;
				if (currWord == "binary_little_endian")	{
					header.m_bBinary = true;
				}
				else {
					header.m_bBinary = false;
				}
			}
			else if (currWord == "property") {
				if (!util::endsWith(line, "vertex_indices") && !util::endsWith(line, "vertex_index")) {
					PlyHeader::PlyPropertyHeader p;
					ss >> p.nameType;
					ss >> p.name;
					if (activeElement == "vertex") {
						if (p.name == "nx")	header.m_bHasNormals = true;
						if (p.name == "red") header.m_bHasColors = true;
					}

					p.byteSize = getByteSizeFromNameType(p.nameType);
					header.m_properties[activeElement].push_back(p);
				}
				// read the line about face indices
				//eg: property list uchar int vertex_indices
				//eg: property list int int vertex_indices
				else if (util::endsWith(line, "vertex_indices") || util::endsWith(line, "vertex_index")) {
					PlyHeader::PlyPropertyHeader p;
					std::string ignore;
					// "list"
					ss >> ignore;
					// uchar/int
					ss >> p.nameType;
					// create our own name for this
					p.name = "verticesPerFace";
					p.byteSize = getByteSizeFromNameType(p.nameType);
					header.m_properties[p.name].push_back(p);
				}
				else {
					//property belonging to unknown element
				}
			}
		}
	};

	struct PlyProperty {
		PlyHeader::PlyPropertyHeader headerInfo;
		std::vector<BYTE> data;
	};

	typedef std::map<std::string, PlyProperty> PlyProperties; //name ->prop

} // namespace ml

#endif