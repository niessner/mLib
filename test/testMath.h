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

		Console::log() << "matrix test passed" << std::endl;
	}

	std::string name()
	{
		return "math";
	}
};