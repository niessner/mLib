class TestMath : public Test
{
public:
	void test0()
	{
		SparseMatrix<double> a("{{2,6,-1},{-9,-7,3}}", MatrixStringFormatMathematica);
		SparseMatrix<double> b("{{3,4,1},{2,5,0},{-6,6,-1},{8,7,-2}}", MatrixStringFormatMathematica);
		SparseMatrix<double> correctProduct("{{29,34,25,60},{-52,-53,9,-127}}", MatrixStringFormatMathematica);
		
		SparseMatrix<double> product = a * b.transpose();
		SparseMatrix<double> sum = a + a;
		SparseMatrix<double> sumCheck = a * 2.0;

		double productError = (product - correctProduct).maxMagnitude();
		MLIB_ASSERT(productError == 0.0, "matrix product test failed");

		double sumError = (sumCheck - sum).maxMagnitude();
		MLIB_ASSERT(sumError == 0.0, "matrix sum test failed");

		LinearSolverConjugateGradient<double> solver;
		Vector<double> rhs = SparseMatrix<double>("{{3,6,-1}}", MatrixStringFormatMathematica).denseRow(0);
		Vector<double> x = solver.solve(b.transpose() * b, rhs);
		double solveError = solver.solveError(b.transpose() * b, x, rhs);

		MLIB_ASSERT(solveError <= 1e-5, "solve failed");

		Console::log() << "matrix test passed" << std::endl;
	}

	void test1()
	{
		SparseMatrix<double> A(100, 10);
		Vector<double> b(100);

		for(UINT row = 0; row < A.rows(); row++)
			for(UINT col = 0; col < A.cols(); col++)
				A(row, col) = ((double)rand() / (double)RAND_MAX - 0.5) * 100.0;

		for(UINT val = 0; val < b.size(); val++)
			b[val] = ((double)rand() / (double)RAND_MAX - 0.5) * 100.0;

		Vector<double> bSquare = A.transpose() * b;

		SparseMatrix<double> ASquare = A.transpose() * A;

		LinearSolverConjugateGradient<double> linearSolver;
		LinearSolverEigen<double> eigenSolver(LinearSolverEigen<double>::ConjugateGradient_Diag);

		Vector<double> x0 = linearSolver.solve(ASquare, bSquare);
		Vector<double> x1 = eigenSolver.solve(ASquare, bSquare);
		Vector<double> x2 = linearSolver.solveLeastSquares(A, b);
		Vector<double> x3 = eigenSolver.solveLeastSquares(A, b);

		double solveError0 = LinearSolver<double>::solveError(ASquare, x0, bSquare);
		double solveError1 = LinearSolver<double>::solveError(ASquare, x1, bSquare);
		double solveError2 = LinearSolver<double>::solveError(ASquare, x2, bSquare);
		double solveError3 = LinearSolver<double>::solveError(ASquare, x3, bSquare);

		MLIB_ASSERT(solveError0 <= 1e-5, "solve failed");
		MLIB_ASSERT(solveError1 <= 1e-5, "solve failed");
		MLIB_ASSERT(solveError2 <= 1e-5, "solve failed");
		MLIB_ASSERT(solveError3 <= 1e-5, "solve failed");

		Console::log() << "Eigen test passed" << std::endl;
	}

	void test2()
	{
		const UINT pointCount = 1000;
		const UINT clusterCount = 10;
		const UINT maxIterations = 25;

		Vector<vec3f> points(pointCount);
		for(int i = 0; i < pointCount; i++)
			points[i] = vec3f(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);

		for(UINT iteration = 0; iteration < 2; iteration++)
		{
			Clock timer;
			KMeansClustering<vec3f, Vec3fKMeansMetric> clustering;
			clustering.cluster(points, clusterCount, maxIterations, false);
			Console::log() << "iteration " << iteration << ": " << timer.elapsed() << std::endl;
		}
		
		Console::log() << "clustering test passed" << std::endl;
	}

	String name()
	{
		return "math";
	}
};