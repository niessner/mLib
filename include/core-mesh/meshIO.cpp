
#ifndef CORE_MESH_MESHIO_INL_H_
#define CORE_MESH_MESHIO_INL_H_

namespace ml {

template <class FloatType>
void MeshIO<FloatType>::loadFromPLY( const std::string& filename, MeshData<FloatType>& mesh, 
	PlyProperties* properties /*= nullptr*/)
{
	mesh.clear();
	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open())	throw MLIB_EXCEPTION("Could not open file " + filename);			

	// read header
	PlyHeader header(file);

	if (header.m_numFaces == (unsigned int)-1) throw MLIB_EXCEPTION("no faces found");
	if (header.m_numVertices == (unsigned int)-1) throw MLIB_EXCEPTION("no vertices found");
	 
	mesh.m_Vertices.resize(header.m_numVertices);
	mesh.m_FaceIndicesVertices.resize(header.m_numFaces, 3);	//assuming triangles
	if (header.m_bHasNormals) mesh.m_Normals.resize(header.m_numVertices);
	if (header.m_bHasColors) mesh.m_Colors.resize(header.m_numVertices);

	unsigned int numExtraProperties = 0;
	if (properties != nullptr) {
		if (!header.m_bBinary) {
			std::cout << "warning: ply properties not supported for ascii" << std::endl;
		}
		else {
			PlyProperties& props = *properties; 
			props.clear();
			std::unordered_set<std::string> standardHeaders = { "x", "y", "z", "nx", "ny", "nz", "red", "green", "blue", "alpha" };
			for (const PlyHeader::PlyPropertyHeader& p : header.m_properties["vertex"]) {
				if (standardHeaders.find(p.name) == standardHeaders.end()) {
					PlyProperty prop;
					prop.headerInfo = p;
					prop.data.resize(header.m_numVertices*p.byteSize);
					props[p.name] = prop;
					numExtraProperties++;
				}
			}
		}
	}

	if (header.m_bBinary)
	{
		//unsigned int size = 3*4+3*4+3+11*4;
		unsigned int size = 0;
		for (unsigned int i = 0; i < header.m_properties["vertex"].size(); i++) {
			size += header.m_properties["vertex"][i].byteSize;
		}
		//size = 3*4+3*4+3+11*4;
		char* data = new char[size*header.m_numVertices];
		file.read(data, size*header.m_numVertices);
		for (unsigned int i = 0; i < header.m_numVertices; i++) {
			unsigned int byteOffset = 0;
			const std::vector<PlyHeader::PlyPropertyHeader>& vertexProperties = header.m_properties["vertex"];

			for (unsigned int j = 0; j < vertexProperties.size(); j++) {
				if (vertexProperties[j].name == "x") {
					mesh.m_Vertices[i].x = ((float*)&data[i*size + byteOffset])[0];
					byteOffset += vertexProperties[j].byteSize;
				}
				else if (vertexProperties[j].name == "y") {
					mesh.m_Vertices[i].y = ((float*)&data[i*size + byteOffset])[0];
					byteOffset += vertexProperties[j].byteSize;
				}
				else if (vertexProperties[j].name == "z") {
					mesh.m_Vertices[i].z = ((float*)&data[i*size + byteOffset])[0];
					byteOffset += vertexProperties[j].byteSize;
				}
				else if (vertexProperties[j].name == "nx") {
					mesh.m_Normals[i].x = ((float*)&data[i*size + byteOffset])[0];
					byteOffset += vertexProperties[j].byteSize;
				}
				else if (vertexProperties[j].name == "ny") {
					mesh.m_Normals[i].y = ((float*)&data[i*size + byteOffset])[0];
					byteOffset += vertexProperties[j].byteSize;
				}
				else if (vertexProperties[j].name == "nz") {
					mesh.m_Normals[i].z = ((float*)&data[i*size + byteOffset])[0];
					byteOffset += vertexProperties[j].byteSize;
				}
				else if (vertexProperties[j].name == "red") {
					mesh.m_Colors[i].x = ((unsigned char*)&data[i*size + byteOffset])[0];	mesh.m_Colors[i].x/=255.0f;
					byteOffset += vertexProperties[j].byteSize;
				}
				else if (vertexProperties[j].name == "green") {
					mesh.m_Colors[i].y = ((unsigned char*)&data[i*size + byteOffset])[0];	mesh.m_Colors[i].y/=255.0f;
					byteOffset += vertexProperties[j].byteSize;
				}
				else if (vertexProperties[j].name == "blue") {
					mesh.m_Colors[i].z = ((unsigned char*)&data[i*size + byteOffset])[0];	mesh.m_Colors[i].z/=255.0f;
					byteOffset += vertexProperties[j].byteSize;
				}
				else if (vertexProperties[j].name == "alpha") {
					mesh.m_Colors[i].w = ((unsigned char*)&data[i*size + byteOffset])[0];	mesh.m_Colors[i].w/=255.0f;
					byteOffset += vertexProperties[j].byteSize;
				} else {
					if (numExtraProperties > 0) {
						PlyProperties& props = *properties;
						auto it = props.find(vertexProperties[j].name);
						if (it != props.end()) {
							memcpy(it->second.data.data() + i*it->second.headerInfo.byteSize, (BYTE*)&data[i*size + byteOffset], it->second.headerInfo.byteSize);
						}
					}
					byteOffset += vertexProperties[j].byteSize; //ignore unknown
				}
			}
			assert(byteOffset == size);

		}	

		delete [] data;

		size = 1+3*4;	//typically 1 uchar for numVertices per triangle, 3 * int for indices
		data = new char[size*header.m_numFaces];
		file.read(data, size*header.m_numFaces);
		for (unsigned int i = 0; i < header.m_numFaces; i++) {	
			mesh.m_FaceIndicesVertices[i][0] = ((int*)&data[i*size+1])[0];
			mesh.m_FaceIndicesVertices[i][1] = ((int*)&data[i*size+1])[1];
			mesh.m_FaceIndicesVertices[i][2] = ((int*)&data[i*size+1])[2];

			//mesh.m_FaceIndicesVertices[i].push_back(((int*)&data[i*size+1])[0]);
			//mesh.m_FaceIndicesVertices[i].push_back(((int*)&data[i*size+1])[1]);
			//mesh.m_FaceIndicesVertices[i].push_back(((int*)&data[i*size+1])[2]);
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
		for (unsigned int i = 0; i < header.m_numVertices; i++) {
			std::string line;
			util::safeGetline(file, line);
			std::stringstream ss(line);	//TOOD replace with sscanf
			ss >> mesh.m_Vertices[i].x >> mesh.m_Vertices[i].y >> mesh.m_Vertices[i].z;
			if (header.m_bHasColors) {
				ss >> mesh.m_Colors[i].x >> mesh.m_Colors[i].y >> mesh.m_Colors[i].z;
				mesh.m_Colors[i] /= (FloatType)255.0;
			}
		}

		for (unsigned int i = 0; i < header.m_numFaces; i++) {
			std::string line;
			util::safeGetline(file, line);
			std::stringstream ss(line); //TOOD replace with sscanf
			unsigned int num_vs;
			ss >> num_vs;
			std::vector<unsigned int> face;	face.reserve(num_vs);
			for (unsigned int j = 0; j < num_vs; j++) {
				unsigned int idx;
				ss >> idx;
				face.push_back(idx);
			}
			mesh.m_FaceIndicesVertices.push_back(face);
		}

		//if (mesh.m_Colors.size() == 0) {
		//	mesh.m_Colors.resize(header.m_NumVertices);
		//	for (size_t i = 0; i < mesh.m_Colors.size(); i++) {
		//		mesh.m_Colors[i] = vec3f(0.5f, 0.5f, 0.5f);
		//	}
		//}

	}
}

template <class FloatType>
void MeshIO<FloatType>::loadFromOFF( const std::string& filename, MeshData<FloatType>& mesh )
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
	//mesh.m_FaceIndicesVertices.resize(numP);
	mesh.m_Colors.resize(numV);

	if(std::string(string1).compare("OFF") == 0)
	{
		// read points
		for(unsigned int i = 0; i < numV; i++) {
			vec3<FloatType> v;
			file >> v.x >> v.y >> v.z;
			mesh.m_Vertices[i] = v;
			mesh.m_Colors[i] = vec4f::origin;
		}
	}
	else
	{	// ignore color
		// read points
		for (unsigned int i = 0; i < numV; i++) {
			vec3<FloatType> v;
			vec4<FloatType> c;
			file >> v.x >> v.y >> v.z;
			file >> c.x >> c.y >> c.z >> c.w;
			mesh.m_Vertices[i] = v;
			mesh.m_Colors[i] = c / 255;	//typically colors are stored in RGB \in [0;255]
		}
	}

	// read faces (i.e., indices)
	for(unsigned int i = 0; i < numP; i++) {
		unsigned int num_vs;
		file >> num_vs;
		std::vector<unsigned int> face(num_vs);
		for (unsigned int j = 0; j < num_vs; j++) {
			unsigned int idx;
			file >> idx;
			face[j] = idx;
		}
		mesh.m_FaceIndicesVertices.push_back(face);
	}
	if (mesh.m_FaceIndicesVertices.size() != numP) throw MLIB_EXCEPTION("error: off loader read wrong number of faces");
}

template <class FloatType>
void MeshIO<FloatType>::loadFromOBJ(const std::string& filename, MeshData<FloatType>& mesh, bool bIgnoreNans)
{
	mesh.clear();

	FILE *fp = nullptr;
	//fopen_s(&fp, filename.c_str(), "r");
	fp = fopen(filename.c_str(), "r");
	if (!fp) throw MLIB_EXCEPTION("Could not open file " + filename);

	char buf[OBJ_LINE_BUF_SIZE];
	float val[6];
	int idx[256][3];
	int match;

	unsigned int type = 0;

	bool bHasFaceNormalIndices = false;
	bool bHasFaceTexCoordIndices = false;

	typename MeshData<FloatType>::GroupIndex activeMaterial;
	typename MeshData<FloatType>::GroupIndex activeGroup;
	bool bActiveMaterial = false;
	bool bActiveGroup = false;
	std::vector<unsigned int> badIndices; //for removing NaN values etc

	while ( fscanf( fp, "%s", buf) != EOF ) {
		if (strncmp(buf, "mtllib", strlen("mtllib")) == 0) {
			if (mesh.m_materialFile.size()) throw MLIB_EXCEPTION("only a single mtllib definition allowed");
			fscanf( fp, "%s", buf);
			mesh.m_materialFile = util::directoryFromPath(filename) + std::string(buf);
		}
		//active material
		else if (strncmp(buf, "usemtl", strlen("usemtl")) == 0) {
			size_t faceIndex = mesh.m_FaceIndicesVertices.size();
			if (bActiveMaterial && activeMaterial.start != faceIndex) {
				activeMaterial.end = faceIndex;
				mesh.m_indicesByMaterial.push_back(activeMaterial);
			}

			fscanf(fp, "%s", buf);
			activeMaterial.name = std::string(buf);
			activeMaterial.start = faceIndex;
			bActiveMaterial = true;
		} 
		else if (buf[0] == 'g') {
			size_t faceIndex = mesh.m_FaceIndicesVertices.size();
			if (bActiveGroup && activeGroup.start != faceIndex) {
				activeGroup.end = faceIndex;
				mesh.m_indicesByGroup.push_back(activeGroup);
			}

			//fscanf(fp, "%s", buf);
			skipLine(buf, OBJ_LINE_BUF_SIZE, fp);
			size_t offset = 0;
			while (buf[offset] == ' ' && buf[offset] != '\0' && offset < OBJ_LINE_BUF_SIZE) offset++;	//skip leading spaces
			activeGroup.name = std::string(buf + offset);
			activeGroup.start = faceIndex;
			bActiveGroup = true;
		}
		else {
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
					if (match >= 3) 
						mesh.m_Vertices.push_back(vec3<FloatType>(val[0], val[1], val[2]));
					else if (bIgnoreNans) {
						badIndices.push_back((unsigned int)mesh.m_Vertices.size());
						mesh.m_Vertices.push_back(vec3<FloatType>(std::numeric_limits<FloatType>::quiet_NaN()));
						mesh.m_Colors.push_back(vec4<FloatType>(std::numeric_limits<FloatType>::quiet_NaN()));
						MLIB_WARNING("warning: bad vert/color format");
					}
					else throw MLIB_EXCEPTION("bad vert format");

					if (match == 6) {  //we found color data
						mesh.m_Colors.push_back(vec4<FloatType>(val[3], val[4], val[5], (FloatType)1.0));
					}
					if (!bIgnoreNans && !(match == 0 || match == 3 || match == 4 || match == 6))
						throw MLIB_EXCEPTION("bad color format");
					break;

				case 'n':
					//normal, 3 components
					match = fscanf( fp, "%f %f %f", &val[0], &val[1], &val[2]);
					mesh.m_Normals.push_back(vec3<FloatType>(val[0], val[1], val[2]));

					if ( match != 3) throw MLIB_EXCEPTION("bad normal format");
					break;

				case 't':
					//texcoord, 2 or 3 components
					val[2] = 0.0f;  //default r coordinate
					match = fscanf( fp, "%f %f %f %f", &val[0], &val[1], &val[2], &val[3]);
					mesh.m_TextureCoords.push_back(vec2<FloatType>(val[0], val[1]));

					//these asserts should actually be there... (commented out because shuran's obj files have NaNs in them...; in that case math == 0)
					//assert( match > 1 && match < 4);
					//assert( match == 2);
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
						idx[0][0] = (idx[0][0] > 0) ? (idx[0][0] - 1) : ((int)mesh.m_Vertices.size() + idx[0][0]);
						idx[0][1] = (idx[0][1] > 0) ? (idx[0][1] - 1) : ((int)mesh.m_Normals.size() + idx[0][1]);

						//grab the second vertex to prime
						fscanf( fp, "%d//%d", &idx[1][0], &idx[1][1]);

						//remap them to the right spot
						idx[1][0] = (idx[1][0] > 0) ? (idx[1][0] - 1) : ((int)mesh.m_Vertices.size() + idx[1][0]);
						idx[1][1] = (idx[1][1] > 0) ? (idx[1][1] - 1) : ((int)mesh.m_Normals.size() + idx[1][1]);

						//create the fan
						while ( fscanf( fp, "%d//%d", &idx[n][0], &idx[n][1]) == 2) {
							//remap them to the right spot
							idx[n][0] = (idx[n][0] > 0) ? (idx[n][0] - 1) : ((int)mesh.m_Vertices.size() + idx[n][0]);
							idx[n][1] = (idx[n][1] > 0) ? (idx[n][1] - 1) : ((int)mesh.m_Normals.size() + idx[n][1]);
							n++;
						}
					}
					else if ( sscanf( buf, "%d/%d/%d", &idx[0][0], &idx[0][1], &idx[0][2]) == 3) {
						type = 3;
						//This face has vertex, texture coordinate, and normal indices

						//remap them to the right spot
						idx[0][0] = (idx[0][0] > 0) ? (idx[0][0] - 1) : ((int)mesh.m_Vertices.size() + idx[0][0]);
						idx[0][1] = (idx[0][1] > 0) ? (idx[0][1] - 1) : ((int)mesh.m_TextureCoords.size() + idx[0][1]);
						idx[0][2] = (idx[0][2] > 0) ? (idx[0][2] - 1) : ((int)mesh.m_Normals.size() + idx[0][2]);

						//grab the second vertex to prime
						fscanf( fp, "%d/%d/%d", &idx[1][0], &idx[1][1], &idx[1][2]);

						//remap them to the right spot
						idx[1][0] = (idx[1][0] > 0) ? (idx[1][0] - 1) : ((int)mesh.m_Vertices.size() + idx[1][0]);
						idx[1][1] = (idx[1][1] > 0) ? (idx[1][1] - 1) : ((int)mesh.m_TextureCoords.size() + idx[1][1]);
						idx[1][2] = (idx[1][2] > 0) ? (idx[1][2] - 1) : ((int)mesh.m_Normals.size() + idx[1][2]);

						//create the fan
						while ( fscanf( fp, "%d/%d/%d", &idx[n][0], &idx[n][1], &idx[n][2]) == 3) {
							//remap them to the right spot
							idx[n][0] = (idx[n][0] > 0) ? (idx[n][0] - 1) : ((int)mesh.m_Vertices.size() + idx[n][0]);
							idx[n][1] = (idx[n][1] > 0) ? (idx[n][1] - 1) : ((int)mesh.m_TextureCoords.size() + idx[n][1]);
							idx[n][2] = (idx[n][2] > 0) ? (idx[n][2] - 1) : ((int)mesh.m_Normals.size() + idx[n][2]);
							n++;
						}
					}
					else if ( sscanf( buf, "%d/%d/", &idx[0][0], &idx[0][1]) == 2) {
						type = 2;
						//This face has vertex and texture coordinate indices

						//remap them to the right spot
						idx[0][0] = (idx[0][0] > 0) ? (idx[0][0] - 1) : ((int)mesh.m_Vertices.size() + idx[0][0]);
						idx[0][1] = (idx[0][1] > 0) ? (idx[0][1] - 1) : ((int)mesh.m_TextureCoords.size() + idx[0][1]);

						//grab the second vertex to prime
						fscanf( fp, "%d/%d/", &idx[1][0], &idx[1][1]);

						//remap them to the right spot
						idx[1][0] = (idx[1][0] > 0) ? (idx[1][0] - 1) : ((int)mesh.m_Vertices.size() + idx[1][0]);
						idx[1][1] = (idx[1][1] > 0) ? (idx[1][1] - 1) : ((int)mesh.m_TextureCoords.size() + idx[1][1]);

						//create the fan
						while ( fscanf( fp, "%d/%d/", &idx[n][0], &idx[n][1]) == 2) {
							//remap them to the right spot
							idx[n][0] = (idx[n][0] > 0) ? (idx[n][0] - 1) : ((int)mesh.m_Vertices.size() + idx[n][0]);
							idx[n][1] = (idx[n][1] > 0) ? (idx[n][1] - 1) : ((int)mesh.m_TextureCoords.size() + idx[n][1]);
							n++;
						}
					}
					else if ( sscanf( buf, "%d/%d", &idx[0][0], &idx[0][1]) == 2) {
						type = 2;
						//This face has vertex and texture coordinate indices

						//remap them to the right spot
						idx[0][0] = (idx[0][0] > 0) ? (idx[0][0] - 1) : ((int)mesh.m_Vertices.size() + idx[0][0]);
						idx[0][1] = (idx[0][1] > 0) ? (idx[0][1] - 1) : ((int)mesh.m_TextureCoords.size() + idx[0][1]);

						//grab the second vertex to prime
						fscanf( fp, "%d/%d", &idx[1][0], &idx[1][1]);

						//remap them to the right spot
						idx[1][0] = (idx[1][0] > 0) ? (idx[1][0] - 1) : ((int)mesh.m_Vertices.size() + idx[1][0]);
						idx[1][1] = (idx[1][1] > 0) ? (idx[1][1] - 1) : ((int)mesh.m_TextureCoords.size() + idx[1][1]);

						//create the fan
						while ( fscanf( fp, "%d/%d", &idx[n][0], &idx[n][1]) == 2) {
							//remap them to the right spot
							idx[n][0] = (idx[n][0] > 0) ? (idx[n][0] - 1) : ((int)mesh.m_Vertices.size() + idx[n][0]);
							idx[n][1] = (idx[n][1] > 0) ? (idx[n][1] - 1) : ((int)mesh.m_TextureCoords.size() + idx[n][1]);
							n++;
						}
					}
					else if ( sscanf( buf, "%d", &idx[0][0]) == 1) {
						type = 1;
						//This face has only vertex indices

						//remap them to the right spot
						idx[0][0] = (idx[0][0] > 0) ? (idx[0][0] - 1) : ((int)mesh.m_Vertices.size() + idx[0][0]);

						//grab the second vertex to prime
						fscanf( fp, "%d", &idx[1][0]);

						//remap them to the right spot
						idx[1][0] = (idx[1][0] > 0) ? (idx[1][0] - 1) : ((int)mesh.m_Vertices.size() + idx[1][0]);

						//create the fan
						while ( fscanf( fp, "%d", &idx[n][0]) == 1) {
							//remap them to the right spot
							idx[n][0] = (idx[n][0] > 0) ? (idx[n][0] - 1) : ((int)mesh.m_Vertices.size() + idx[n][0]);
							n++;
						}
					}
					else {
						throw MLIB_EXCEPTION(filename + ": broken obj (face line invalid)");
						//skipLine( buf, OBJ_LINE_BUF_SIZE, fp);
					}

					if (n < 3)	
						throw MLIB_EXCEPTION(filename + ": broken obj (face with less than 3 indices)");

					//create face
					std::vector<unsigned int> currFaceIndicesVertices;
					std::vector<unsigned int> currFaceIndicesNormals;
					std::vector<unsigned int> currFaceIndicesTextureCoords;

					for (unsigned int i = 0; i < n; i++) {	
						currFaceIndicesVertices.push_back(idx[i][0]);
						if (type == 2) {		//has vertex, tex coords
							currFaceIndicesTextureCoords.push_back(idx[i][1]);
						} else if (type == 3) { //has vertex, tex coords, normals
							currFaceIndicesTextureCoords.push_back(idx[i][1]);
							currFaceIndicesNormals.push_back(idx[i][2]);
						} else if (type == 4) { //has vertex, normals
							currFaceIndicesNormals.push_back(idx[i][1]);
						}
					}
				

					if (currFaceIndicesVertices.size())			mesh.m_FaceIndicesVertices.push_back(currFaceIndicesVertices);

					if (currFaceIndicesNormals.size()) {
						mesh.m_FaceIndicesNormals.push_back(currFaceIndicesNormals);
						bHasFaceNormalIndices = true;
					} else {
						//insert empty face
						typename MeshData<FloatType>::Indices::Face fFaceIndicesNormals(nullptr, 0);
						mesh.m_FaceIndicesNormals.push_back(fFaceIndicesNormals);
					}
					if (currFaceIndicesTextureCoords.size()) {
						mesh.m_FaceIndicesTextureCoords.push_back(currFaceIndicesTextureCoords);
						bHasFaceTexCoordIndices = true;
					} else {
						//insert empty face
						typename MeshData<FloatType>::Indices::Face fFaceIndicesTextureCoords(nullptr, 0);
						mesh.m_FaceIndicesTextureCoords.push_back(fFaceIndicesTextureCoords);
					}
				}

				break;

			case 's':
			//case 'g':	//groups are handled above and result int different materials
			case 'u':
				//all presently ignored
			default:
				skipLine( buf, OBJ_LINE_BUF_SIZE, fp);

			};
		}
	}
	if (!badIndices.empty() && mesh.m_Colors.size() == badIndices.size()) {
		mesh.m_Colors.clear();
	}

	fclose(fp);

	if (!bHasFaceTexCoordIndices)	mesh.m_FaceIndicesTextureCoords.clear();
	if (!bHasFaceNormalIndices)		mesh.m_FaceIndicesNormals.clear();


	if (bActiveMaterial) {
		size_t faceIndex = mesh.m_FaceIndicesVertices.size();
		activeMaterial.end = faceIndex;
		mesh.m_indicesByMaterial.push_back(activeMaterial);
	}
	if (bActiveGroup) {
		size_t faceIndex = mesh.m_FaceIndicesVertices.size();
		activeGroup.end = faceIndex;
		mesh.m_indicesByGroup.push_back(activeGroup);
	}

}




template <class FloatType>
void MeshIO<FloatType>::saveToPLY( const std::string& filename, const MeshData<FloatType>& mesh, 
	const PlyProperties* properties /*= nullptr*/)
{
	if (!std::is_same<FloatType, float>::value) throw MLIB_EXCEPTION("only implemented for float, not for double");

	std::ofstream file(filename, std::ios::binary);
	if (!file.is_open()) throw MLIB_EXCEPTION("Could not open file for writing " + filename);
	file << "ply\n";
	file << "format binary_little_endian 1.0\n";
	file << "comment MLIB generated\n";
	file << "element vertex " << mesh.m_Vertices.size() << "\n";
	file << "property float x\n";
	file << "property float y\n";
	file << "property float z\n";
	if (mesh.m_Normals.size() > 0) {
		file << "property float nx\n";
		file << "property float ny\n";
		file << "property float nz\n";
	}
	if (mesh.m_Colors.size() > 0) {
		file << "property uchar red\n";
		file << "property uchar green\n";
		file << "property uchar blue\n";
		file << "property uchar alpha\n";
	}
	if (properties != nullptr) {
		for (const auto& p : *properties) 
			file << "property " << p.second.headerInfo.nameType << " " << p.second.headerInfo.name << "\n";
	}
	file << "element face " << mesh.m_FaceIndicesVertices.size() << "\n";
	file << "property list uchar int vertex_indices\n";
	file << "end_header\n";

	//TODO make this more efficient: i.e., copy first into an array, and then perform just a single write
	if (mesh.m_Colors.size() > 0 || mesh.m_Normals.size() > 0) {
		//for (size_t i = 0; i < mesh.m_Vertices.size(); i++) {
		//	file.write((const char*)&mesh.m_Vertices[i], sizeof(float)*3);
		//	if (mesh.m_Normals.size() > 0) {
		//		file.write((const char*)&mesh.m_Normals[i], sizeof(float)*3);
		//	}
		//	if (mesh.m_Colors.size() > 0) {
		//		vec4uc c(mesh.m_Colors[i]*255.0f);
		//		file.write((const char*)&c, sizeof(unsigned char)*4);
		//	}
		//}

		size_t vertexByteSize = sizeof(float)*3;
		if (mesh.m_Normals.size() > 0)	vertexByteSize += sizeof(float)*3;
		if (mesh.m_Colors.size() > 0)	vertexByteSize += sizeof(unsigned char)*4;
		if (properties != nullptr) {
			for (const auto& p : *properties) vertexByteSize += p.second.headerInfo.byteSize;
		}
		BYTE* data = new BYTE[vertexByteSize*mesh.m_Vertices.size()];
		size_t byteOffset = 0;
		for (size_t i = 0; i < mesh.m_Vertices.size(); i++) {
			memcpy(&data[byteOffset], &mesh.m_Vertices[i], sizeof(float)*3);
			byteOffset += sizeof(float)*3;
			if (mesh.m_Normals.size() > 0) {
				memcpy(&data[byteOffset], &mesh.m_Normals[i], sizeof(float)*3);
				byteOffset += sizeof(float)*3;
			}
			if (mesh.m_Colors.size() > 0) {
				vec4uc c(mesh.m_Colors[i]*255);
				memcpy(&data[byteOffset], &c, sizeof(unsigned char)*4);
				byteOffset += sizeof(unsigned char)*4;
			}
			if (properties != nullptr) {
				for (const auto& p : *properties) {
					memcpy(&data[byteOffset], p.second.data.data()+i*p.second.headerInfo.byteSize, p.second.headerInfo.byteSize);
					byteOffset += p.second.headerInfo.byteSize;
				}
			}
		}
		file.write((const char*)data, byteOffset);
		SAFE_DELETE_ARRAY(data);
	} else {
		file.write((const char*)&mesh.m_Vertices[0], sizeof(float)*3*mesh.m_Vertices.size());
	}
	for (size_t i = 0; i < mesh.m_FaceIndicesVertices.size(); i++) {
		unsigned char numFaceIndices = (unsigned char)mesh.m_FaceIndicesVertices[i].size();
		file.write((const char*)&numFaceIndices, sizeof(unsigned char));
		file.write((const char*)&mesh.m_FaceIndicesVertices[i][0], numFaceIndices*sizeof(unsigned int));
	}
	file.close();
}


template <class FloatType>
void MeshIO<FloatType>::saveToOFF( const std::string& filename, const MeshData<FloatType>& mesh )
{
	std::ofstream file(filename);
	if (!file.is_open())	throw MLIB_EXCEPTION("Could not open file for writing " + filename);		

	// first line should say 'OFF'
	file << "OFF\n";

	// write header (verts, faces, edges)
	file << mesh.m_Vertices.size() << " " << mesh.m_FaceIndicesVertices.size() << " " << 0 << "\n";

	// write points
	for (unsigned int i = 0; i < mesh.m_Vertices.size(); i++) {
		file << mesh.m_Vertices[i].x << " " << mesh.m_Vertices[i].y << " " << mesh.m_Vertices[i].z;
		if (mesh.m_Colors.size() > 0) {
			file << " " << 
				(unsigned int)(mesh.m_Colors[i].x*255) << " " << 
				(unsigned int)(mesh.m_Colors[i].y*255) << " " << 
				(unsigned int)(mesh.m_Colors[i].z*255) << " " << 
				(unsigned int)(mesh.m_Colors[i].w*255) << " ";
		}
		file << "\n";
	}

	// write faces
	for (unsigned int i = 0; i < mesh.m_FaceIndicesVertices.size(); i++) {
		file << mesh.m_FaceIndicesVertices[i].size();
		for (unsigned int j = 0; j < mesh.m_FaceIndicesVertices[i].size(); j++) {
			file << " " << mesh.m_FaceIndicesVertices[i][j];
		}
		file << "\n";
	}

	file.close();
}


template <class FloatType>
void MeshIO<FloatType>::saveToOBJ( const std::string& filename, const MeshData<FloatType>& mesh )
{
	std::ofstream file(filename);
	if (!file.is_open())	throw MLIB_EXCEPTION("Could not open file for writing " + filename);

	file << "####\n";
	file << "#\n";
	file << "# OBJ file Generated by MLIB\n";
	file << "#\n";
	file << "####\n";
	file << "# Object " << filename << "\n";
	file << "#\n";
	file << "# Vertices: " << mesh.m_Vertices.size() << "\n";
	file << "# Faces: " << mesh.m_FaceIndicesVertices.size() << "\n";
	file << "#\n";
	file << "####\n";

	if (!mesh.m_materialFile.empty()) {
		file << "mtllib " << util::splitPath(mesh.m_materialFile).back() << "\n";
	}

	for (size_t i = 0; i < mesh.m_Vertices.size(); i++) {
		file << "v ";
		if (isnan(mesh.m_Vertices[i].x))	file << "NaN NaN NaN";
		else								file << mesh.m_Vertices[i].x << " " << mesh.m_Vertices[i].y << " " << mesh.m_Vertices[i].z;
		if (mesh.m_Colors.size() > 0) {
			if (isnan(mesh.m_Colors[i].x))	file << " NaN NaN NaN";
			else							file << " " << mesh.m_Colors[i].x << " " << mesh.m_Colors[i].y << " " << mesh.m_Colors[i].z;
		}
		file << "\n";
	}
	for (size_t i = 0; i < mesh.m_Normals.size(); i++) {
		file << "vn ";
		file << mesh.m_Normals[i].x << " " << mesh.m_Normals[i].y << " " << mesh.m_Normals[i].z << "\n";
	}
	for (size_t i = 0; i < mesh.m_TextureCoords.size(); i++) {
		file << "vt ";
		file << mesh.m_TextureCoords[i].x << " " << mesh.m_TextureCoords[i].y << "\n";
	}
	unsigned int currMaterialIdx = 0;
	unsigned int currGroupIdx = 0;
	for (unsigned int i = 0; i < mesh.m_FaceIndicesVertices.size(); i++) {
		if (!mesh.m_indicesByGroup.empty()) {
			if (mesh.m_indicesByGroup[currGroupIdx].end == i)
				currGroupIdx++;
			if (mesh.m_indicesByGroup[currGroupIdx].start == i)
				file << "g " << mesh.m_indicesByGroup[currGroupIdx].name << "\n";
		}		
		if (!mesh.m_indicesByMaterial.empty()) {
			if (mesh.m_indicesByMaterial[currMaterialIdx].end == i) 
				currMaterialIdx++;
			if (mesh.m_indicesByMaterial[currMaterialIdx].start == i)
				file << "usemtl " << mesh.m_indicesByMaterial[currMaterialIdx].name << "\n";
		}

		file << "f ";
		for (unsigned int j = 0; j < mesh.m_FaceIndicesVertices[i].size(); j++) {
			file << mesh.m_FaceIndicesVertices[i][j]+1;

			if (mesh.hasTexCoords() || mesh.hasNormals()) {
				file << "/";
				if (mesh.hasTexCoords()) {
					if (mesh.hasTexCoordsIndices()) file << mesh.m_FaceIndicesTextureCoords[i][j] + 1;
					else if (mesh.hasVertexIndices())  file << mesh.m_FaceIndicesVertices[i][j] + 1;	//in this case the indicesTexCoords and indicesTexCoords are identical
				}
				file << "/";
				if (mesh.hasNormals()) {
					if (mesh.hasNormalIndices()) file << mesh.m_FaceIndicesNormals[i][j] + 1;
					else if (mesh.hasVertexIndices())	file << mesh.m_FaceIndicesVertices[i][j] + 1;	//in this case the indicesNormals and indicesTexCoords are identical
				}				
			}
			//if (mesh.m_FaceIndicesTextureCoords.size() > 0 || mesh.m_FaceIndicesNormals.size() > 0) {
			//	file << "/";
			//	if (mesh.m_FaceIndicesTextureCoords.size() > 0) {
			//		file << mesh.m_FaceIndicesTextureCoords[i][j]+1;
			//	}
			//	file << "/";
			//	if (mesh.m_FaceIndicesNormals.size() > 0) {
			//		file << mesh.m_FaceIndicesNormals[i][j]+1;
			//	}
			//}
			file << " ";
		}
		file << "\n";
	}

	file.close();
}

}  // namespace ml

#endif  // CORE_MESH_MESHIO_INL_H_