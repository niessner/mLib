
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
    std::vector<T*> eigenvectorList()
    {
        std::vector<T*> result(eigenvectors.rows());
        for (rowIndex = 0; rowIndex < eigenvectors.rows(); rowIndex++)
            result[rowIndex] = &eigenvectors(rowIndex, 0);
        return result;
    }
    DenseMatrix<T> eigenvectors;
    MathVector<T> eigenvalues;
};

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
    void eigenTest(const DenseMatrix<T> &M, const T **eigenvectors, const T *eigenvalues) const
    {
        const unsigned int n = M.rows();
        const std::vector<T> eigenvector(n);
        double maxError = 0.0;
        for (UINT eigenIndex = 0; eigenIndex < n; eigenIndex++)
        {
            for (UINT element = 0; element < n; element++)
                eigenvector[element] = eigenvectors[element][eigenIndex];

            std::vector<T> result = multiply(M, eigenvector);
            
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
    void eigenSystem(const DenseMatrix<T> &M, T **eigenvectors, T *eigenvalues) const;
};

}  // namespace ml

#include "eigenSolver.inl"

#endif  // CORE_MATH_EIGENSOLVER_H_
