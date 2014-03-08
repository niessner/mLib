
#pragma once
#ifndef _LINEAR_SOLVER_H_
#define _LINEAR_SOLVER_H_

template<class D> class LinearSolver
{
public:
	virtual std::vector<D> solve(const SparseMatrix<D> &A, const std::vector<D> &b) = 0;
	virtual std::vector<D> solveLeastSquares(const SparseMatrix<D> &A, const std::vector<D> &b) = 0;
	static double solveError(const SparseMatrix<D> &A, const std::vector<D> &x, const std::vector<D> &b)
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

	std::vector<D> solve(const SparseMatrix<D> &A, const std::vector<D> &b)
	{
		MLIB_ASSERT_STR(A.square() && b.size() == A.rows(), "invalid solve dimensions");
		const UINT n = (UINT)b.size();

		std::vector<D> dInverse = A.diagonal().map([](D x) {return (D)1.0 / x;});

		std::vector<D> x(n, 0.0);
		std::vector<D> r = b - A * x;
		std::vector<D> z = std::vector<D>::directProduct(dInverse, r);
		std::vector<D> p = z;

		for(UINT iteration = 0; iteration < m_maxIterations; iteration++)
		{
			D gamma = std::vector<D>::dotProduct(r, z);
			if(fabs(gamma) < 1e-20) break;
			D alpha = gamma / SparseMatrix<D>::quadratic(A, p);
			x = x + alpha * p;
			r = r - alpha * (A * p);
			if(r.maxValue() <= m_tolerance && r.minIndex() >= -m_tolerance) break;
			z = std::vector<D>::directProduct(dInverse, r);

			D beta = std::vector<D>::dotProduct(z, r) / gamma;
			p = z + beta * p;
		}
		return x;
	}

	std::vector<D> solveLeastSquares(const SparseMatrix<D> &A, const std::vector<D> &b)
	{
		auto Atranspose = A.transpose();
		return solve(Atranspose * A, Atranspose * b);
	}

private:
	UINT m_maxIterations;
	double m_tolerance;
};

#endif // _LINEAR_SOLVER_H_