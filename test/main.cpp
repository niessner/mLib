
#define MLIB_ERROR_CHECK

#include "main.h"

using namespace ml;


class App
{
public:
	void go();

private:
	TestGrid m_grid;
	TestUtility m_utility;
	//TestMath m_math;
	TestANN m_ANN;
	TestLodePNG m_lodePNG;
	TestBinaryStream m_binaryStream;
	TestOpenMesh m_openMesh;
};

void App::go()
{
	ml::Console::openLogFile("console.txt");
	m_utility.run();
	//m_math.run();
	m_ANN.run();
	m_lodePNG.run();
	m_binaryStream.run();
	m_grid.run();
	m_openMesh.run();

	ml::Console::log("All tests completed");
	std::cin.get();
}
 
int main()
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

#endif
	{
		vec3f p0(0,1,0);
		vec3f p1(1,0,0);
		quatf q(p0, p1);
		mat3f m0 = q.matrix3x3();
		mat4f m1 = q.matrix4x4();

		vec3f res0 = q*p0;
		vec3f res1 = m0*p0;
		vec3f res2 = m1*p0;

		quatf re0(m0);
		quatf re1(m1);
	}
	{
		mat3f m0 = mat3f::rotation(vec3f(0,0,1), 90);
		vec3f p0(1,0,0);
		vec3f p1(0,1,0);
		quatf q(p0, p1);
		mat3f m1 = q.matrix3x3();
		std::cout << m0*p0 << std::endl;
		std::cout << mat3f::rotationZ(90)*p0 << std::endl;
		std::cout << quatf(vec3f(0,0,1), 90)*p0 << std::endl;
		std::cout << q*p0 << std::endl;
		std::cout << q.matrix3x3()*p0 << std::endl;
		std::cout << quatf(q.matrix3x3())*p0 << std::endl;
		std::cout << std::endl;
	}
	{
		mat3f m0 = mat3f::rotation(vec3f(0,1,0), 90);
		vec3f p0(1,0,0);
		vec3f p1(0,0,-1);
		quatf q(p0, p1);
		mat3f m1 = q.matrix3x3();
		std::cout << m0*p0 << std::endl;
		std::cout << mat3f::rotationY(90)*p0 << std::endl;
		std::cout << quatf(vec3f(0,1,0), 90)*p0 << std::endl;
		std::cout << q*p0 << std::endl;
		std::cout << q.matrix3x3()*p0 << std::endl;
		std::cout << quatf(q.matrix3x3())*p0 << std::endl;
		std::cout << std::endl;
	}
	{
		mat3f m0 = mat3f::rotation(vec3f(1,0,0), 90);
		vec3f p0(0,1,0);
		vec3f p1(0,0,1);
		quatf q(p0, p1);
		mat3f m1 = q.matrix3x3();
		std::cout << m0*p0 << std::endl;
		std::cout << mat3f::rotationX(90)*p0 << std::endl;
		std::cout << quatf(vec3f(1,0,0), 90)*p0 << std::endl;
		std::cout << q*p0 << std::endl;
		std::cout << q.matrix3x3()*p0 << std::endl;
		std::cout << quatf(q.matrix3x3())*p0 << std::endl;
		std::cout << std::endl;
	} 


	/*
	//ml::ColorImageR8G8B8 test;
	//ml::FreeImageWrapper::loadImage("test1.png", test);
	//ml::FreeImageWrapper::saveImage("out1.jpg", test);

	const int numElements = 256*1024;
	int* data = new int[numElements]; //1024 byte of data
	const size_t numBuckets = 1024*4 + 1;
	//const size_t numBuckets = 2;
	
	ml::BinaryDataStreamFile streamOut("test.out", true);
	for (size_t i = 0; i < numElements; i++) {
		data[i] = (int)i;
	}
	//const size_t numBuckets = 10;
	for (size_t i = 0; i < numBuckets; i++) {
		streamOut.writeData((const BYTE*)&data[0], sizeof(int)*numElements);
	}
	streamOut.flush();
	streamOut.closeStream();
	std::cout << "write done" << std::endl;

	std::cout << "filesize: " << ml::util::getFileSize("test.out") << std::endl;

	ml::BinaryDataStreamFile streamIn("test.out", false);
	for (size_t i = 0; i < numBuckets; i++) {
		streamIn.readData((BYTE*)&data[0], sizeof(int)*numElements);
		for (SIZE_T j = 0; j < numElements; j++) {
			if (data[j] != j) std::cout << "i " << i << " error match while read: " << data[50] << std::endl;
		}
	}
	std::cout << "read done" << std::endl;
	SAFE_DELETE_ARRAY(data);
	//ml::SparseGrid3D<float> signedDistanceField, in;
	//for (unsigned int i = 0; i < 5; i++) {
	//	ml::vec3i c(i, 2*i, 3*i);
	//	signedDistanceField(c) = (float)i+5.0f; 
	//}
	//s << signedDistanceField;
	//s >> in;
	//std::cout << signedDistanceField << std::endl << std::endl << in << std::endl;

	//std::cout << std::endl << std::endl;
	//signedDistanceField.writeBinaryDump("bla.dump");
	//ml::SparseGrid3D<float> reRead;
	//reRead.readBinaryDump("bla.dump");
	//std::cout << reRead << std::endl;
	*/
	App a;
	a.go();

	getchar();
	return 0;
}
