#ifndef _MESH_IO_H_
#define _MESH_IO_H_


template <class FloatType>
class MeshIO {

public:

	static MeshData<FloatType> loadFromFile(const std::string& filename) {
		MeshData<FloatType> data;	
		loadFromFile(filename, data);
		return data;
	}

	static void loadFromFile(const std::string& filename, MeshData<FloatType>& mesh) {
		mesh.clear();
		std::string extension = StringUtil::getFileExtension(filename);

		if (extension == "off") {
			loadFromFileOFF(filename, mesh);
		} else if (extension == "ply") {
			loadFromPLY(filename, mesh);
		} else if (extension == "obj") {
			loadFromFileOBJ(filename, mesh);
		} else 	{
			throw MLIB_EXCEPTION("unknown file format: " + filename);
		}

		if (!mesh.isConsistent()) {
			throw MLIB_EXCEPTION("inconsistent mesh data: " + filename);
		}
	}

	static void writeToFile(const std::string& filename, const MeshData<FloatType>& mesh) {

		if (!mesh.isConsistent()) {
			throw MLIB_EXCEPTION("inconsistent mesh data: " + filename);
		}

		std::string extension = StringUtil::getFileExtension(filename);

		if (extension == "off") {

		} else if (extension == "ply") {

		} else if (extension == "obj") {

		} else {
			throw MLIB_EXCEPTION("unknown file format: " + filename);
		}
	}



	/************************************************************************/
	/* Write Functions													    */
	/************************************************************************/



	/************************************************************************/
	/* Read Functions													    */
	/************************************************************************/



	static void loadFromPLY(const std::string& filename, MeshData<FloatType>& mesh);

	static void loadFromFileOFF(const std::string& filename, MeshData<FloatType>& mesh);

	static void loadFromFileOBJ(const std::string& filename, MeshData<FloatType>& mesh);
private:


#define OBJ_LINE_BUF_SIZE 256
	static void skipLine(char * buf, int size, FILE * fp)
	{
		do {
			buf[size-1] = '$';
			fgets(buf, size, fp);
		} while (buf[size-1] != '$');
	}

	struct PlyHeader {
		PlyHeader() {
			m_NumVertices = -1;
			m_NumFaces = -1;
		}
		unsigned int m_NumVertices;
		unsigned int m_NumFaces;
		bool m_binary;
	};

	static void PlyHeaderLine(const std::string& line, PlyHeader& header) {

		std::stringstream ss(line);
		std::string currWord;
		ss >> currWord;

		if (currWord == "element") {
			ss >> currWord;
			if (currWord == "vertex") {
				ss >> header.m_NumVertices;
			} else if (currWord == "face") {
				ss >> header.m_NumFaces;
			}
		} 

		if(currWord == "format") {
			ss >> currWord;
			if (currWord == "binary_little_endian")	{
				header.m_binary = true;
			} else {
				header.m_binary = false;
			}
		}
	}
};

typedef MeshIO<float>	MeshIOf;
typedef MeshIO<double>	MeshIOd;


#include "meshIO.cpp"

#endif