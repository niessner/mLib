
#ifndef CORE_MESH_MESHIO_INL_H_
#define CORE_MESH_MESHIO_INL_H_

namespace ml {

template <class FloatType>
void MeshIO<FloatType>::loadFromPLY( const std::string& filename, MeshData<FloatType>& mesh )
{
	mesh.clear();
	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open())	throw MLIB_EXCEPTION("Could not open file " + filename);			

	// read header
	PlyHeader header;

	std::string line;
	std::getline(file, line);
	while (line.find("end_header") == std::string::npos) {
		PlyHeaderLine(line, header);
		std::getline(file, line);
	}

	assert(header.m_NumFaces != -1);
	assert(header.m_NumVertices != -1);

	mesh.m_Vertices.resize(header.m_NumVertices);
	mesh.m_FaceIndicesVertices.resize(header.m_NumFaces);
	if (header.m_bHasNormals) mesh.m_Normals.resize(header.m_NumVertices);
	if (header.m_bHasColors) mesh.m_Colors.resize(header.m_NumVertices);

	if(header.m_bBinary)
	{
		//unsigned int size = 3*4+3*4+3+11*4;
		unsigned int size = 0;
		for (unsigned int i = 0; i < header.m_Properties.size(); i++) {
			size += header.m_Properties[i].byteSize;
		}
		//size = 3*4+3*4+3+11*4;
		char* data = new char[size*header.m_NumVertices];
		file.read(data, size*header.m_NumVertices);
		for (unsigned int i = 0; i < header.m_NumVertices; i++) {
			unsigned int byteOffset = 0;
			for (unsigned int j = 0; j < header.m_Properties.size(); j++) {
				if (header.m_Properties[j].name == "x") {
					mesh.m_Vertices[i].x = ((float*)&data[i*size + byteOffset])[0];
					byteOffset += header.m_Properties[j].byteSize;
				}
				else if (header.m_Properties[j].name == "y") {
					mesh.m_Vertices[i].y = ((float*)&data[i*size + byteOffset])[0];
					byteOffset += header.m_Properties[j].byteSize;
				}
				else if (header.m_Properties[j].name == "z") {
					mesh.m_Vertices[i].z = ((float*)&data[i*size + byteOffset])[0];
					byteOffset += header.m_Properties[j].byteSize;
				}
				else if (header.m_Properties[j].name == "nx") {
					mesh.m_Normals[i].x = ((float*)&data[i*size + byteOffset])[0];
					byteOffset += header.m_Properties[j].byteSize;
				}
				else if (header.m_Properties[j].name == "ny") {
					mesh.m_Normals[i].y = ((float*)&data[i*size + byteOffset])[0];
					byteOffset += header.m_Properties[j].byteSize;
				}
				else if (header.m_Properties[j].name == "nz") {
					mesh.m_Normals[i].z = ((float*)&data[i*size + byteOffset])[0];
					byteOffset += header.m_Properties[j].byteSize;
				}
				else if (header.m_Properties[j].name == "red") {
					mesh.m_Colors[i].x = ((unsigned char*)&data[i*size + byteOffset])[0];	mesh.m_Colors[i].x/=255.0f;
					byteOffset += header.m_Properties[j].byteSize;
				}
				else if (header.m_Properties[j].name == "green") {
					mesh.m_Colors[i].y = ((unsigned char*)&data[i*size + byteOffset])[0];	mesh.m_Colors[i].y/=255.0f;
					byteOffset += header.m_Properties[j].byteSize;
				}
				else if (header.m_Properties[j].name == "blue") {
					mesh.m_Colors[i].z = ((unsigned char*)&data[i*size + byteOffset])[0];	mesh.m_Colors[i].z/=255.0f;
					byteOffset += header.m_Properties[j].byteSize;
				}
				else if (header.m_Properties[j].name == "alpha") {
					//mesh.m_Colors[i].z = ((unsigned char*)&data[i*size + byteOffset])[0];	mesh.m_Colors[i].z/=255.0f;
					byteOffset += header.m_Properties[j].byteSize;
				}
			}
			assert(byteOffset == size);

		}	

		delete [] data;

		size = 1+3*4;	//tyiically 1 uchar for numVertices per triangle, 3 * int for indeices
		data = new char[size*header.m_NumFaces];
		file.read(data, size*header.m_NumFaces);
		for (unsigned int i = 0; i < header.m_NumFaces; i++) {	
			mesh.m_FaceIndicesVertices[i].push_back(((int*)&data[i*size+1])[0]);
			mesh.m_FaceIndicesVertices[i].push_back(((int*)&data[i*size+1])[1]);
			mesh.m_FaceIndicesVertices[i].push_back(((int*)&data[i*size+1])[2]);
		}

		//if (mesh.m_Colors.size() == 0) {
		//	mesh.m_Colors.resize(header.m_NumVertices);
		//	for (size_t i = 0; i < mesh.m_Colors.size(); i++) {
		//		mesh.m_Colors[i] = vec3f(0.5f, 0.5f, 0.5f);
		//	}
		//}
		delete [] data;

	}
	else
	{
		//TODO fis this ;)
		for (unsigned int i = 0; i < header.m_NumVertices; i++) {
			std::getline(file, line);
			std::stringstream ss(line);
			ss >> mesh.m_Vertices[i].x >> mesh.m_Vertices[i].y >> mesh.m_Vertices[i].z;
		}

		for (unsigned int i = 0; i < header.m_NumFaces; i++) {
			std::getline(file, line);
			std::stringstream ss(line);
			unsigned int num_vs;
			ss >> num_vs;
			for (unsigned int j = 0; j < num_vs; j++) {
				unsigned int idx;
				ss >> idx;
				mesh.m_FaceIndicesVertices[i].push_back(idx);
			}
		}

		if (mesh.m_Colors.size() == 0) {
			mesh.m_Colors.resize(header.m_NumVertices);
			for (size_t i = 0; i < mesh.m_Colors.size(); i++) {
				mesh.m_Colors[i] = vec3f(0.5f, 0.5f, 0.5f);
			}
		}

	}
}

template <class FloatType>
void MeshIO<FloatType>::loadFromFileOFF( const std::string& filename, MeshData<FloatType>& mesh )
{
	mesh.clear();

	std::ifstream file(filename);
	if (!file.is_open())	throw MLIB_EXCEPTION("Could not open file " + filename);			

	// first line should say 'OFF'
	char string1[5];
	file >> string1;

	// read header
	unsigned int numV = 0;
	unsigned int numP = 0;
	unsigned int numE = 0;
	file >> numV >> numP >> numE;

	mesh.m_Vertices.resize(numV);
	mesh.m_FaceIndicesVertices.resize(numP);
	mesh.m_Colors.resize(numV);

	if(std::string(string1).compare("OFF") == 0)
	{
		// read points
		for(unsigned int i = 0; i < numV; i++) {
			point3d<FloatType> v;
			file >> v.x >> v.y >> v.z;
			mesh.m_Vertices[i] = v;
			mesh.m_Colors[i] = vec3f(0.0f, 0.0f, 0.0f);
		}
	}
	else
	{	// ignore color
		unsigned int dump;
		// read points
		for(unsigned int i = 0; i < numV; i++) {
			point3d<FloatType> v;
			point3d<FloatType> c;
			file >> v.x >> v.y >> v.z;
			file >> c.x >> c.y >> c.z >> dump;
			mesh.m_Vertices[i] = v;
			mesh.m_Colors[i] = c / 255;	//typically colors are stored in RGB \in [0;255]
		}
	}

	// read faces (i.e., indices)
	for(unsigned int i = 0; i < numP; i++) {
		unsigned int num_vs;
		file >> num_vs;
		for (unsigned int j = 0; j < num_vs; j++) {
			unsigned int idx;
			file >> idx;
			mesh.m_FaceIndicesVertices[i].push_back(idx);
		}
	}
}

template <class FloatType>
void MeshIO<FloatType>::loadFromFileOBJ( const std::string& filename, MeshData<FloatType>& mesh )
{
	mesh.clear();

	FILE *fp = NULL;
	//fopen_s(&fp, filename.c_str(), "r");
	fp = fopen(filename.c_str(), "r");
	if (!fp) throw MLIB_EXCEPTION("Could not open file " + filename);

	char buf[OBJ_LINE_BUF_SIZE];
	float val[6];
	int idx[256][3];
	int match;

	unsigned int type = 0;


	while ( fscanf( fp, "%s", buf) != EOF ) {

		switch (buf[0]) {
		case '#':
			//comment line, eat the remainder
			skipLine( buf, OBJ_LINE_BUF_SIZE, fp);
			break;
		case 'v':
			switch (buf[1]) {

			case '\0':
				//vertex, 3 or 4 components
				val[3] = 1.0f;  //default w coordinate
				match = fscanf( fp, "%f %f %f %f %f %f", &val[0], &val[1], &val[2], &val[3], &val[4], &val[5]);	//meshlab stores colors right after vertex pos (3 xyz, 3 rgb)
				mesh.m_Vertices.push_back(point3d<FloatType>(val[0], val[1], val[2]));

				if (match == 6) {  //we found color data
					mesh.m_Colors.push_back(point3d<FloatType>(val[3], val[4], val[5]));
				}
				assert( match == 3 || match == 4 || match == 6);
				break;

			case 'n':
				//normal, 3 components
				match = fscanf( fp, "%f %f %f", &val[0], &val[1], &val[2]);
				mesh.m_Normals.push_back(point3d<FloatType>(val[0], val[1], val[2]));

				assert( match == 3);
				break;

			case 't':
				//texcoord, 2 or 3 components
				val[2] = 0.0f;  //default r coordinate
				match = fscanf( fp, "%f %f %f %f", &val[0], &val[1], &val[2], &val[3]);
				mesh.m_TextureCoords.push_back(point2d<FloatType>(val[0], val[1]));

				assert( match > 1 && match < 4);
				assert( match == 2);
				break;
			}
			break;

		case 'f':
			//face
			fscanf( fp, "%s", buf);
			{
				unsigned int n = 2;

				//determine the type, and read the initial vertex, all entries in a face must have the same format
				if ( sscanf( buf, "%d//%d", &idx[0][0], &idx[0][1]) == 2) {
					type = 4;
					//This face has vertex and normal indices

					//remap them to the right spot
					idx[0][0] = (idx[0][0] > 0) ? (idx[0][0] - 1) : ((int)mesh.m_Vertices.size() - idx[0][0]);
					idx[0][1] = (idx[0][1] > 0) ? (idx[0][1] - 1) : ((int)mesh.m_Normals.size() - idx[0][1]);

					//grab the second vertex to prime
					fscanf( fp, "%d//%d", &idx[1][0], &idx[1][1]);

					//remap them to the right spot
					idx[1][0] = (idx[1][0] > 0) ? (idx[1][0] - 1) : ((int)mesh.m_Vertices.size() - idx[1][0]);
					idx[1][1] = (idx[1][1] > 0) ? (idx[1][1] - 1) : ((int)mesh.m_Normals.size() - idx[1][1]);

					//create the fan
					while ( fscanf( fp, "%d//%d", &idx[n][0], &idx[n][1]) == 2) {
						//remap them to the right spot
						idx[n][0] = (idx[n][0] > 0) ? (idx[n][0] - 1) : ((int)mesh.m_Vertices.size() - idx[n][0]);
						idx[n][1] = (idx[n][1] > 0) ? (idx[n][1] - 1) : ((int)mesh.m_Normals.size() - idx[n][1]);
						n++;
					}
				}
				else if ( sscanf( buf, "%d/%d/%d", &idx[0][0], &idx[0][1], &idx[0][2]) == 3) {
					type = 3;
					//This face has vertex, texture coordinate, and normal indices

					//remap them to the right spot
					idx[0][0] = (idx[0][0] > 0) ? (idx[0][0] - 1) : ((int)mesh.m_Vertices.size() - idx[0][0]);
					idx[0][1] = (idx[0][1] > 0) ? (idx[0][1] - 1) : ((int)mesh.m_TextureCoords.size() - idx[0][1]);
					idx[0][2] = (idx[0][2] > 0) ? (idx[0][2] - 1) : ((int)mesh.m_Normals.size() - idx[0][2]);

					//grab the second vertex to prime
					fscanf( fp, "%d/%d/%d", &idx[1][0], &idx[1][1], &idx[1][2]);

					//remap them to the right spot
					idx[1][0] = (idx[1][0] > 0) ? (idx[1][0] - 1) : ((int)mesh.m_Vertices.size() - idx[1][0]);
					idx[1][1] = (idx[1][1] > 0) ? (idx[1][1] - 1) : ((int)mesh.m_TextureCoords.size() - idx[1][1]);
					idx[1][2] = (idx[1][2] > 0) ? (idx[1][2] - 1) : ((int)mesh.m_Normals.size() - idx[1][2]);

					//create the fan
					while ( fscanf( fp, "%d/%d/%d", &idx[n][0], &idx[n][1], &idx[n][2]) == 3) {
						//remap them to the right spot
						idx[n][0] = (idx[n][0] > 0) ? (idx[n][0] - 1) : ((int)mesh.m_Vertices.size() - idx[n][0]);
						idx[n][1] = (idx[n][1] > 0) ? (idx[n][1] - 1) : ((int)mesh.m_TextureCoords.size() - idx[n][1]);
						idx[n][2] = (idx[n][2] > 0) ? (idx[n][2] - 1) : ((int)mesh.m_Normals.size() - idx[n][2]);
						n++;
					}
				}
				else if ( sscanf( buf, "%d/%d/", &idx[0][0], &idx[0][1]) == 2) {
					type = 2;
					//This face has vertex and texture coordinate indices

					//remap them to the right spot
					idx[0][0] = (idx[0][0] > 0) ? (idx[0][0] - 1) : ((int)mesh.m_Vertices.size() - idx[0][0]);
					idx[0][1] = (idx[0][1] > 0) ? (idx[0][1] - 1) : ((int)mesh.m_TextureCoords.size() - idx[0][1]);

					//grab the second vertex to prime
					fscanf( fp, "%d/%d/", &idx[1][0], &idx[1][1]);

					//remap them to the right spot
					idx[1][0] = (idx[1][0] > 0) ? (idx[1][0] - 1) : ((int)mesh.m_Vertices.size() - idx[1][0]);
					idx[1][1] = (idx[1][1] > 0) ? (idx[1][1] - 1) : ((int)mesh.m_TextureCoords.size() - idx[1][1]);

					//create the fan
					while ( fscanf( fp, "%d/%d/", &idx[n][0], &idx[n][1]) == 2) {
						//remap them to the right spot
						idx[n][0] = (idx[n][0] > 0) ? (idx[n][0] - 1) : ((int)mesh.m_Vertices.size() - idx[n][0]);
						idx[n][1] = (idx[n][1] > 0) ? (idx[n][1] - 1) : ((int)mesh.m_TextureCoords.size() - idx[n][1]);
						n++;
					}
				}
				else if ( sscanf( buf, "%d/%d", &idx[0][0], &idx[0][1]) == 2) {
					type = 2;
					//This face has vertex and texture coordinate indices

					//remap them to the right spot
					idx[0][0] = (idx[0][0] > 0) ? (idx[0][0] - 1) : ((int)mesh.m_Vertices.size() - idx[0][0]);
					idx[0][1] = (idx[0][1] > 0) ? (idx[0][1] - 1) : ((int)mesh.m_TextureCoords.size() - idx[0][1]);

					//grab the second vertex to prime
					fscanf( fp, "%d/%d", &idx[1][0], &idx[1][1]);

					//remap them to the right spot
					idx[1][0] = (idx[1][0] > 0) ? (idx[1][0] - 1) : ((int)mesh.m_Vertices.size() - idx[1][0]);
					idx[1][1] = (idx[1][1] > 0) ? (idx[1][1] - 1) : ((int)mesh.m_TextureCoords.size() - idx[1][1]);

					//create the fan
					while ( fscanf( fp, "%d/%d", &idx[n][0], &idx[n][1]) == 2) {
						//remap them to the right spot
						idx[n][0] = (idx[n][0] > 0) ? (idx[n][0] - 1) : ((int)mesh.m_Vertices.size() - idx[n][0]);
						idx[n][1] = (idx[n][1] > 0) ? (idx[n][1] - 1) : ((int)mesh.m_TextureCoords.size() - idx[n][1]);
						n++;
					}
				}
				else if ( sscanf( buf, "%d", &idx[0][0]) == 1) {
					type = 1;
					//This face has only vertex indices

					//remap them to the right spot
					idx[0][0] = (idx[0][0] > 0) ? (idx[0][0] - 1) : ((int)mesh.m_Vertices.size() - idx[0][0]);

					//grab the second vertex to prime
					fscanf( fp, "%d", &idx[1][0]);

					//remap them to the right spot
					idx[1][0] = (idx[1][0] > 0) ? (idx[1][0] - 1) : ((int)mesh.m_Vertices.size() - idx[1][0]);

					//create the fan
					while ( fscanf( fp, "%d", &idx[n][0]) == 1) {
						//remap them to the right spot
						idx[n][0] = (idx[n][0] > 0) ? (idx[n][0] - 1) : ((int)mesh.m_Vertices.size() - idx[n][0]);
						n++;
					}
				}
				else {
					throw MLIB_EXCEPTION(filename + ": broken obj (face line invalid)");
					//skipLine( buf, OBJ_LINE_BUF_SIZE, fp);
				}

				if (n < 3)	throw MLIB_EXCEPTION(filename + ": broken obj (face with less than 3 indices)");

				//create face
				std::vector<unsigned int> currFaceIndicesVertices;
				std::vector<unsigned int> currFaceIndicesNormals;
				std::vector<unsigned int> currFaceIndicesTextureCoords;

				for (unsigned int i = 0; i < n; i++) {	
					currFaceIndicesVertices.push_back(idx[i][0]);
					if (type == 2) {	//has vertex and tex coords
						currFaceIndicesTextureCoords.push_back(idx[i][1]);
					} else if (type == 3) {
						currFaceIndicesTextureCoords.push_back(idx[i][1]);
						currFaceIndicesNormals.push_back(idx[i][2]);
					}
					//vertex[i] = vertices[idx[i][0]];
					//	if (type == 2) {	//has vertex and tex coords
					//		vertex[i]->texCoord = float2(_texCoords[idx[i][1]*3+0], _texCoords[idx[i][1]*3+1]);
					//	} else if (type == 3) { // has vertex, normals and tex coords
					//		vertex[i]->texCoord = float2(_texCoords[idx[i][2]*3+0], _texCoords[idx[i][2]*3+1]);
					//	}
				}

				if (currFaceIndicesVertices.size())			mesh.m_FaceIndicesVertices.push_back(currFaceIndicesVertices);
				if (currFaceIndicesNormals.size())			mesh.m_FaceIndicesNormals.push_back(currFaceIndicesNormals);
				if (currFaceIndicesTextureCoords.size())	mesh.m_FaceIndicesTextureCoords.push_back(currFaceIndicesTextureCoords);
			}

			break;

		case 's':
		case 'g':
		case 'u':
			//all presently ignored
		default:
			skipLine( buf, OBJ_LINE_BUF_SIZE, fp);

		};
	}

	fclose(fp);
}

}  // namespace ml

#endif  // CORE_MESH_MESHIO_INL_H_