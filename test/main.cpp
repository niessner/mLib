
#include "main.h"

using namespace ml;

class App
{
public:
	void go();

private:
	TestUtility m_utility;
	TestMath m_math;
	TestANN m_ANN;
	TestLodePNG m_lodePNG;
	TestBinaryStream m_binaryStream;
};

void App::go()
{
	ml::Console::openLogFile("console.txt");
	m_utility.run();
	m_math.run();
	m_ANN.run();
	m_lodePNG.run();
	m_binaryStream.run();

	ml::Console::log("All tests completed");
	std::cin.get();
}
  
int main()
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	vec3f p0(0,0,1);
	vec3f p1(0,1,0);
	quatf q(p0, p1);
	mat3f m = q.matrix3x3();


	int c = 5;

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
