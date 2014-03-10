
#ifndef CORE_MATH_LINEARSOLVER_H_
#define CORE_MATH_LINEARSOLVER_H_

namespace ml {

template<class D> class LinearSolver
{
public:
	virtual MathVector<D> solve(const SparseMatrix<D> &A, const MathVector<D> &b) = 0;
	virtual MathVector<D> solveLeastSquares(const SparseMatrix<D> &A, const MathVector<D> &b) = 0;
	static D solveError(const SparseMatrix<D> &A, const MathVector<D> &x, const MathVector<D> &b)
	{
		//.map([](T n) {return std::string(n);})
		//return (A * x - b).map([](D x) {return fabs(x);}).maxValue();
		D res = (D)0.0;
		std::vector<D> Ax = A*x;
		for (size_t i = 0; i < Ax.size(); i++) {
			res +=  (Ax[i] - b[i])*(Ax[i] - b[i]);
		} 
		return std::sqrt(res);
	}
};

template<class D> class LinearSolverConjugateGradient : public LinearSolver<D>
{
public:
	LinearSolverConjugateGradient(UINT maxIterations = 10000, D tolerance = 1e-10)
	{
		m_maxIterations = maxIterations;
		m_tolerance = tolerance;
	}

	MathVector<D> solve(const SparseMatrix<D> &A, const MathVector<D> &b)
	{
		MLIB_ASSERT_STR(A.square() && b.size() == A.rows(), "invalid solve dimensions");
		const UINT n = (UINT)b.size();

		//std::vector<D> dInverse = A.diagonal().map([](D x) {return (D)1.0 / x;});
		MathVector<D> dInverse = A.diagonal();
		auto invert = [=] (D& x) { x = (D)1.0/x; };
		for_each(dInverse.begin(), dInverse.end(), invert);

		MathVector<D> x(n, 0.0);
		MathVector<D> r = b - A * x;
		MathVector<D> z = dInverse * r;
		MathVector<D> p = z;

		for(UINT iteration = 0; iteration < m_maxIterations; iteration++)
		{
			D gamma = r | z;
			if(fabs(gamma) < 1e-20) break;
			D alpha = gamma / SparseMatrix<D>::quadratic(A, p);
			x = x + alpha * p;
			r = r - alpha * (A * p);

			if (*std::max_element(r.begin(), r.end()) <= m_tolerance && *std::min_element(r.begin(), r.end()) >= -m_tolerance)	break;

			z = dInverse * r;
			D beta = (z | r) / gamma;
			p = z + beta * p;
		}
		return x;
	}

	MathVector<D> solveLeastSquares(const SparseMatrix<D> &A, const MathVector<D> &b)
	{
		auto Atranspose = A.transpose();
		return solve(Atranspose * A, Atranspose * b);
	}

private:
	UINT m_maxIterations;
	D m_tolerance;
};

}  // namespace ml

#endif  // CORE_MATH_LINEARSOLVER_H_
