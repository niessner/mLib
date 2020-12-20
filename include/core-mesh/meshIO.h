
#ifndef CORE_MESH_MESHIO_H_
#define CORE_MESH_MESHIO_H_

#include <string>
#include <core-util/utility.h>
#include <core-mesh/meshData.h>

namespace ml {

template <class FloatType>
class MeshIO {

public:

	static MeshData<FloatType> loadFromFile(const std::string& filename, bool bIgnoreNans = false) {
		MeshData<FloatType> data;	
		loadFromFile(filename, data, bIgnoreNans);
		return data;
	}

	static void loadFromFile(const std::string& filename, MeshData<FloatType>& mesh, bool bIgnoreNans = false) {
		mesh.clear();
		std::string extension = util::getFileExtension(filename);

		if (extension == "off") {
			loadFromOFF(filename, mesh);
		} else if (extension == "ply") {
			loadFromPLY(filename, mesh);
		} else if (extension == "obj") {
			loadFromOBJ(filename, mesh, bIgnoreNans);
		} else 	{
			throw MLIB_EXCEPTION("unknown file format: " + filename);
		}

		//mesh.deleteRedundantIndices();

		if (!mesh.isConsistent()) {
			throw MLIB_EXCEPTION("inconsistent mesh data: " + filename);
		}
	}

	static void saveToFile(const std::string& filename, const MeshData<FloatType>& mesh) {

		if (mesh.isEmpty()) {		
			MLIB_WARNING("empty mesh: " + filename);
			return;
		}

		if (!mesh.isConsistent()) {
			throw MLIB_EXCEPTION("inconsistent mesh data: " + filename);
		}

		std::string extension = util::getFileExtension(filename);

		if (extension == "off") {
			saveToOFF(filename, mesh);
		} else if (extension == "ply") {
			saveToPLY(filename, mesh);
		} else if (extension == "obj") {
			saveToOBJ(filename, mesh);
		} else {
			throw MLIB_EXCEPTION("unknown file format: " + filename);
		}
	}


	/************************************************************************/
	/* Read Functions													    */
	/************************************************************************/

	static void loadFromPLY(const std::string& filename, MeshData<FloatType>& mesh, PlyProperties* properties = nullptr); //vertex properties only

	static void loadFromOFF(const std::string& filename, MeshData<FloatType>& mesh);

	static void loadFromOBJ(const std::string& filename, MeshData<FloatType>& mesh, bool bIgnoreNans);


	/************************************************************************/
	/* Write Functions													    */
	/************************************************************************/

	static void saveToPLY(const std::string& filename, const MeshData<FloatType>& mesh, const PlyProperties* properties = nullptr); //vertex properties only

	static void saveToOFF(const std::string& filename, const MeshData<FloatType>& mesh);

	static void saveToOBJ(const std::string& filename, const MeshData<FloatType>& mesh);

private:

#define OBJ_LINE_BUF_SIZE 256
	static void skipLine(char* buf, int size, FILE* fp)
	{
		//some weird files don't have newlines, which confuses fgets
		size_t curr = 0;
		buf[curr] = '\0';
		while (1) {
			int c = fgetc(fp);
			if (c == EOF || c == '\n' || c == '\r') break;
			if (curr < OBJ_LINE_BUF_SIZE - 1) {
				buf[curr] = (char)c;
				curr++;
			}
		}
		buf[curr] = '\0';

		//do {
		//	buf[size-1] = '$';
		//	fgets(buf, size, fp);
		//} while (buf[size-1] != '$');
	}
};

typedef MeshIO<float>	MeshIOf;
typedef MeshIO<double>	MeshIOd;

}  // namespace ml

#include "meshIO.cpp"

#endif  // CORE_MESH_MESHIO_H_