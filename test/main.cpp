
#define MLIB_ERROR_CHECK

#include "main.h"

using namespace ml;


class App
{
public:
	void go();

private:
	TestBox m_box;
	TestCGAL m_cgal;
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
	m_box.run();
	m_cgal.run();
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

void foo(int var) {
	while (1)
		std::cout << __FUNCTION__ << var << std::endl;
}
void pause_thread(int n)
{
	std::this_thread::sleep_for(std::chrono::seconds(n));
	std::cout << "pause of " << n << " seconds ended\n";
}


class A {
public:
	A() {
		int a = 5;
	}
	~A() {
		int a = 5;
	}
private:
};

class B : public A {
public:
	B() {}
	~B() {}
private:

};

int main()
{
	{
		ColorImageR8G8B8 image(2, 1);
		image(0, 0) = 0.0f;
		image(1, 0) = 1.0f;
		image.resize(11, 1, false);
		for (unsigned int i = 0; i < image.getWidth(); i++) {
			std::cout << image(i, 0u) << std::endl;
		}
		

		auto img = image.getResized(10, 10);

		int a = 5;
		vec3f bla;
	}
	{

		ColorImageR32G32B32A32 image;
		FreeImageWrapper::loadImage("ak.png", image);
		image.resize(100, 100, false);
		FreeImageWrapper::saveImage("test_out_nn.png", image);

		FreeImageWrapper::loadImage("ak.png", image);
		while (image.getWidth() > 100) {
			image.resize(image.getWidth()-1, image.getHeight()-1, true);
		}
		
		FreeImageWrapper::saveImage("test_out_bi.png", image);

		std::cout << "loading done!" << std::endl;
		getchar();
	}



	MeshDataf md;
	MeshIOf::loadFromFile("dining1d.ply", md);
	MeshIOf::saveToFile("test.ply", md);

	std::cout << "loading done!" << std::endl;
	getchar();

	PointCloudf pc;
	PointCloudIOf::loadFromFile("cloud_bin_1.ply", pc);
	PointCloudIOf::saveToFile("test.ply", pc);


	std::cout << "loading done!" << std::endl;
	getchar();

	DistanceField3f df(1, 1, 1);
	B* b = new B();

	delete b;

	Grid3f* grid = new Grid3f(1, 1, 1);
	SAFE_DELETE(grid);

	{
		std::cout << "Spawning and detaching 3 threads...\n";
		std::thread (pause_thread, 1).detach();
		std::thread (pause_thread, 2).detach();
		std::thread (pause_thread, 3).detach();
		std::cout << "Done spawning threads.\n";
	}
	std::cout << "(the main thread will now pause for 5 seconds)\n";
	// give the detached threads time to finish (but not guaranteed!):
	pause_thread(5);


	getchar();
	std::thread t1(foo, 1);
	std::thread t2(foo, 2);


	getchar();
	vec3f v(1.0f, 2.0f, 3.0f);
	vec3ul c = v;

	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#endif

	ColorImageR32G32B32 image(2, 2);
	image(0, 0) = (vec3f)RGBColor::Red.getVec3() / 255.0f;
	image(0, 1) = (vec3f)RGBColor::Blue.getVec3() / 255.0f;
	image(1, 0) = (vec3f)RGBColor::Green.getVec3() / 255.0f;
	image(1, 1) = (vec3f)RGBColor::Yellow.getVec3() / 255.0f;
	FreeImageWrapper::saveImage("bla.png", *(Image*)&image, true);

	//{
	// REMOVE ISOLATED VERTICES TEST
	//	MeshDataf meshData = MeshIOf::loadFromFile("dining1d.ply");
	//	meshData.removeIsolatedVertices();
	//	MeshIOf::saveToPLY("dining1d_removed.ply", meshData);

	//	std::cout << "done" << std::endl;
	//	getchar();
	//}

	//{
	// KABSCH TEST
	//	std::vector<vec3f> source, target;
	//	mat4f trans; trans.setIdentity();
	//	trans = mat4f::translation(vec3f(1, 2, 3));
	//	trans = trans * mat4f::rotation(vec3f(1,2,3).getNormalized(), 25);
	//	RNG rng;
	//	for (unsigned int i = 0; i < 3; i++) {
	//		vec3f p = vec3f((float)rng.rand_closed01(), (float)rng.rand_closed01(), (float)rng.rand_closed01());
	//		source.push_back(p);
	//		target.push_back(trans * p);

	//		target[i] += (float)rng.rand_closed01() / 2;
	//	}




	//	mat4f res;
	//	Timer t;
	//	for (unsigned int i = 0; i < 1; i++) {
	//		res = EigenWrapperf::kabsch(source, target);
	//	}
	//	std::cout << "time required " << t.getElapsedTimeMS() << std::endl;
	//	std::cout << trans << std::endl;
	//	std::cout << res << std::endl;

	//	float error = EigenWrapperf::reProjectionError(source, target, res);

	//	std::cout << "reProjection Error: " << error << std::endl;
	//}


	//{
	//	mat2f m(1, 2, 3, 4);
	//	//EigenSystemf es = m.eigenSystem();
	//	//es.sortByAbsValue();
	//	//vec2f ev0(es.eigenvectorList()[0]);
	//	//vec2f ev1(es.eigenvectorList()[1]);


	//	Eigen::MatrixXd me;
	//	eigenutil::makeEigenMatrix(DenseMatrixf(m), me);
	//	DenseMatrixf ret;
	//	eigenutil::dumpEigenMatrix(me, ret);
	//	std::cout << ret << std::endl;

	//	std::cout << me << std::endl;
	//	
	//	
	//	EigenSystemf es = EigenSolverf::solve<EigenSolverf::TYPE_EIGEN>(m);
	//	vec2f ev0(es.eigenvectorList()[0]);
	//	vec2f ev1(es.eigenvectorList()[1]);
	//	mat2f check = mat2f(ev0, ev1)  * mat2f::diag(es.eigenvalues[0], es.eigenvalues[1]) * mat2f(ev0, ev1).getInverse();

	//	int a = 5;

	//}

	//{
	//	vec3f p0(0,1,0);
	//	vec3f p1(1,0,0);
	//	quatf q(p0, p1);
	//	mat3f m0 = q.matrix3x3();
	//	mat4f m1 = q.matrix4x4();

	//	vec3f res0 = q*p0;
	//	vec3f res1 = m0*p0;
	//	vec3f res2 = m1*p0;

	//	quatf re0(m0);
	//	quatf re1(m1);
	//}
	//{
	//	mat3f m0 = mat3f::rotation(vec3f(0,0,1), 90);
	//	vec3f p0(1,0,0);
	//	vec3f p1(0,1,0);
	//	quatf q(p0, p1);
	//	mat3f m1 = q.matrix3x3();
	//	std::cout << m0*p0 << std::endl;
	//	std::cout << mat3f::rotationZ(90)*p0 << std::endl;
	//	std::cout << quatf(vec3f(0,0,1), 90)*p0 << std::endl;
	//	std::cout << q*p0 << std::endl;
	//	std::cout << q.matrix3x3()*p0 << std::endl;
	//	std::cout << quatf(q.matrix3x3())*p0 << std::endl;
	//	std::cout << std::endl;
	//}
	//{
	//	mat3f m0 = mat3f::rotation(vec3f(0,1,0), 90);
	//	vec3f p0(1,0,0);
	//	vec3f p1(0,0,-1);
	//	quatf q(p0, p1);
	//	mat3f m1 = q.matrix3x3();
	//	std::cout << m0*p0 << std::endl;
	//	std::cout << mat3f::rotationY(90)*p0 << std::endl;
	//	std::cout << quatf(vec3f(0,1,0), 90)*p0 << std::endl;
	//	std::cout << q*p0 << std::endl;
	//	std::cout << q.matrix3x3()*p0 << std::endl;
	//	std::cout << quatf(q.matrix3x3())*p0 << std::endl;
	//	std::cout << std::endl;
	//}
	//{
	//	mat3f m0 = mat3f::rotation(vec3f(1,0,0), 90);
	//	vec3f p0(0,1,0);
	//	vec3f p1(0,0,1);
	//	quatf q(p0, p1);
	//	mat3f m1 = q.matrix3x3();
	//	std::cout << m0*p0 << std::endl;
	//	std::cout << mat3f::rotationX(90)*p0 << std::endl;
	//	std::cout << quatf(vec3f(1,0,0), 90)*p0 << std::endl;
	//	std::cout << q*p0 << std::endl;
	//	std::cout << q.matrix3x3()*p0 << std::endl;
	//	std::cout << quatf(q.matrix3x3())*p0 << std::endl;
	//	std::cout << std::endl;
	//} 


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
