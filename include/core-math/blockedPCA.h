namespace ml {

template <class T>
class BlockedPCA
{
public:
	struct Subset
	{
		Subset() {
			startDim = std::numeric_limits<int>::min();
			dimCount = 0;
		}
		PCA<T> pca;
		int startDim;
		int dimCount;
	};

	BlockedPCA() {}

	typedef std::function<EigenSystem<T>(const DenseMatrix<T> &m)> EigenSolverFunc;

    // points is a matrix with dimensions (# data points, # dimensions)
    void init(const DenseMatrix<T> &points, size_t subsetCount, const EigenSolverFunc &eigenSolver);
    
    void save(const std::string &baseFilename) const;
    void load(const std::string &baseFilename);

    void transform(const T *input, size_t reducedSubsetDimension, T *result) const;
    void inverseTransform(const T *input, size_t reducedSubsetDimension, T *result) const;

private:

	std::vector< Subset > _subsets;
};

#include "blockedPCA.cpp"

typedef BlockedPCA<float> BlockedPCAf;
typedef BlockedPCA<double> BlockedPCAd;

}
