class TestANN : public Test
{
public:
	void test0()
	{
		NearestNeighborSearchBruteForce<double> bruteForce;
		NearestNeighborSearchANN<double> ANN;

		const UINT dimension = 5;
		const UINT maxK = 20;

		Vector< Vector<double> > points;
		for(UINT p = 0; p < 1000; p++)
		{
			points.pushBack();
			for(UINT d = 0; d < dimension; d++)
				points.back().pushBack((double)rand() / (double)RAND_MAX);
		}

		bruteForce.init(points, maxK);
		ANN.init(points, maxK);

		for(UINT queryIndex = 0; queryIndex < 100; queryIndex++)
		{
			Vector<double> randomPt;
			for(UINT d = 0; d < dimension; d++)
				randomPt.pushBack((double)rand() / (double)RAND_MAX);

			Vector<UINT> resultA = bruteForce.kNearest(randomPt, maxK, 0.0);
			Vector<UINT> resultB = ANN.kNearest(randomPt, maxK, 0.0);

			MLIB_ASSERT(resultA == resultB, "NN search inconsistent");
		}
	}

	std::string name()
	{
		return "ANN";
	}
};