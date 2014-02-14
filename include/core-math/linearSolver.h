
#pragma once

template<class D> class LinearSolver
{
public:
	virtual Vector<D> solve(const SparseMatrix<D> &A, const Vector<D> &b) = 0;
	double solveError(const SparseMatrix<D> &A, const Vector<D> &x, const Vector<D> &b)
	{
		//.map([](T n) {return String(n);})
		return (A * x - b).map([](D x) {return fabs(x);}).maxValue();
	}
};

template<class D> class LinearSolverConjugateGradient : public LinearSolver<D>
{
public:
	LinearSolverConjugateGradient(UINT _maxIterations = 10000, double _tolerance = 1e-5)
	{
		maxIterations = _maxIterations;
		tolerance = _tolerance;
	}
	Vector<D> solve(const SparseMatrix<D> &A, const Vector<D> &b)
	{
		MLIB_ASSERT(A.square() && b.size() == A.rows(), "invalid solve dimensions");
		const UINT n = (UINT)b.size();

		Vector<D> dInverse = A.diagonal().map([](D x) {return (D)1.0 / x;});

		Vector<D> x(n, 0.0);
		Vector<D> r = b - A * x;
		Vector<D> z = Vector<D>::directProduct(dInverse, r);
		Vector<D> p = z;

		for(UINT iteration = 0; iteration < maxIterations; iteration++)
		{
			D gamma = Vector<D>::dotProduct(r, z);
			if(fabs(gamma) < 1e-10) break;
			D alpha = gamma / SparseMatrix<D>::quadratic(A, p);
			x = x + alpha * p;
			r = r - alpha * (A * p);
			if(r.maxValue() <= tolerance && r.minIndex() >= tolerance) break;
			z = Vector<D>::directProduct(dInverse, r);

			D beta = Vector<D>::dotProduct(z, r) / gamma;
			p = z + beta * p;
		}
		return x;
	}

	UINT maxIterations;
	double tolerance;
};
