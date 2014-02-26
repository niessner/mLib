
#pragma once
#ifndef _LINEAR_SOLVER_H_
#define _LINEAR_SOLVER_H_

template<class D> class LinearSolver
{
public:
	virtual Vector<D> solve(const SparseMatrix<D> &A, const Vector<D> &b) = 0;
	virtual Vector<D> solveLeastSquares(const SparseMatrix<D> &A, const Vector<D> &b) = 0;
	static double solveError(const SparseMatrix<D> &A, const Vector<D> &x, const Vector<D> &b)
	{
		//.map([](T n) {return std::string(n);})
		return (A * x - b).map([](D x) {return fabs(x);}).maxValue();
	}
};

template<class D> class LinearSolverConjugateGradient : public LinearSolver<D>
{
public:
	LinearSolverConjugateGradient(UINT maxIterations = 10000, double tolerance = 1e-10)
	{
		m_maxIterations = maxIterations;
		m_tolerance = tolerance;
	}

	Vector<D> solve(const SparseMatrix<D> &A, const Vector<D> &b)
	{
		MLIB_ASSERT_STR(A.square() && b.size() == A.rows(), "invalid solve dimensions");
		const UINT n = (UINT)b.size();

		Vector<D> dInverse = A.diagonal().map([](D x) {return (D)1.0 / x;});

		Vector<D> x(n, 0.0);
		Vector<D> r = b - A * x;
		Vector<D> z = Vector<D>::directProduct(dInverse, r);
		Vector<D> p = z;

		for(UINT iteration = 0; iteration < m_maxIterations; iteration++)
		{
			D gamma = Vector<D>::dotProduct(r, z);
			if(fabs(gamma) < 1e-20) break;
			D alpha = gamma / SparseMatrix<D>::quadratic(A, p);
			x = x + alpha * p;
			r = r - alpha * (A * p);
			if(r.maxValue() <= m_tolerance && r.minIndex() >= -m_tolerance) break;
			z = Vector<D>::directProduct(dInverse, r);

			D beta = Vector<D>::dotProduct(z, r) / gamma;
			p = z + beta * p;
		}
		return x;
	}

	Vector<D> solveLeastSquares(const SparseMatrix<D> &A, const Vector<D> &b)
	{
		auto Atranspose = A.transpose();
		return solve(Atranspose * A, Atranspose * b);
	}

private:
	UINT m_maxIterations;
	double m_tolerance;
};

#endif // _LINEAR_SOLVER_H_