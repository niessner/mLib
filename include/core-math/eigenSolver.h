
#ifndef CORE_MATH_EIGENSOLVER_H_
#define CORE_MATH_EIGENSOLVER_H_

namespace ml {

//
// All eigensystem code is generally stateless and could be done with static functions. Virtual classes
// are used instead of static functions to enforce a common interface.
//

template<class T>
struct EigenSystem
{
    EigenSystem(int n)
    {
        eigenvectors = DenseMatrix<T>(n, n);
        eigenvalues.resize(n);
    }
	EigenSystem(EigenSystem&& e) {
		eigenvectors = std::move(e.eigenvectors);
		eigenvalues = std::move(e.eigenvalues);
	}
	void operator=(EigenSystem&& e) {
		eigenvectors = std::move(e.eigenvectors);
		eigenvalues = std::move(e.eigenvalues);
	}

    std::vector<T*> eigenvectorList()
    {
        std::vector<T*> result(eigenvectors.rows());
        for (unsigned int row = 0; row < eigenvectors.rows(); row++)
            result[row] = &eigenvectors(row, 0);
        return result;
    }
    DenseMatrix<T> eigenvectors;
    MathVector<T> eigenvalues;
};

typedef EigenSystem<float> EigenSystemf;
typedef EigenSystem<double> EigenSystemd;

template<class FloatType>
std::ostream& operator<<(std::ostream& s, const EigenSystem<FloatType>& e) {
	s << e.eigenvectors << std::endl << e.eigenvalues << std::endl;
	return s;
}

template<class T> class EigenSolver
{
public:
    
    EigenSystem<T> eigenSystem(const DenseMatrix<T> &M) const
    {
        EigenSystem<T> result(M.rows());
        eigenSystemInternal(M, &result.eigenvectorList()[0], &result.eigenvalues[0]);
        return result;
    }
    void eigenSystem(const DenseMatrix<T> &M, T **eigenvectors, T *eigenvalues) const
    {
        eigenSystemInternal(M, eigenvalues, eigenvectors);
    }
    void eigenTest(const DenseMatrix<T> &M) const
    {
        EigenSystem<T> result(M.rows());
        eigenSystemInternal(M, &result.eigenvectorList()[0], &result.eigenvalues[0]);
        eigenTest(M, &result.eigenvectorList()[0], &result.eigenvalues[0]);
    }
    void eigenTest(const DenseMatrix<T> &M, T **eigenvectors, T *eigenvalues) const
    {
        const unsigned int n = M.rows();
        std::vector<T> eigenvector(n);
        double maxError = 0.0;
        for (UINT eigenIndex = 0; eigenIndex < n; eigenIndex++)
        {
            for (UINT element = 0; element < n; element++)
                eigenvector[element] = eigenvectors[element][eigenIndex];

            std::vector<T> result = DenseMatrix<T>::multiply(M, eigenvector);
            
            double error = 0.0;
            T eigenvalue = eigenvalues[eigenIndex];
            for (UINT element = 0; element < n; element++)
                error += fabs(eigenvector[element] * eigenvalue - result[element]);
            
            maxError = std::max(maxError, error);
        }
        std::cout << "max eigenvector error: " << maxError << std::endl;
    }

private:
    virtual void eigenSystemInternal(const DenseMatrix<T> &M, T **eigenvectors, T *eigenvalues) const = 0;
};

template<class T> class EigenSolverVTK : public EigenSolver<T>
{
public:
    void eigenSystemInternal(const DenseMatrix<T> &M, T **eigenvectors, T *eigenvalues) const;
};

template<class T> class EigenSolverNR : public  EigenSolver<T>
{
public:
	void eigenSystemInternal(const DenseMatrix<T> &M, T **eigenvectors, T *eigenvalues) const {
		//TODO MATTHIAS here
	}
};

}  // namespace ml

#include "eigenSolver.inl"

#endif  // CORE_MATH_EIGENSOLVER_H_
