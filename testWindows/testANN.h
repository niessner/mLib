class TestANN : public Test
{
public:
	void test0()
	{
		using namespace ml;
		NearestNeighborSearchBruteForce<double> bruteForce;
		NearestNeighborSearchANN<double> ANN;

		const UINT dimension = 5;
		const UINT maxK = 20;

		std::vector< std::vector<double> > points;
		for(UINT p = 0; p < 1000; p++)
		{
			points.push_back( std::vector<double>() );
			for(UINT d = 0; d < dimension; d++)
				points.back().push_back((double)rand() / (double)RAND_MAX);
		}

		bruteForce.init(points, maxK);
		ANN.init(points, maxK);

		for(UINT queryIndex = 0; queryIndex < 100; queryIndex++)
		{
			std::vector<double> randomPt;
			for(UINT d = 0; d < dimension; d++)
				randomPt.push_back((double)rand() / (double)RAND_MAX);

			std::vector<UINT> resultA = bruteForce.kNearest(randomPt, maxK, 0.0);
			std::vector<UINT> resultB = ANN.kNearest(randomPt, maxK, 0.0);

			MLIB_ASSERT_STR(resultA == resultB, "NN search inconsistent");
		}
	}

	std::string name()
	{
		return "ANN";
	}
};