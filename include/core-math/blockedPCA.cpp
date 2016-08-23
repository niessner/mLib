
template<class T>
void BlockedPCA<T>::init(const DenseMatrix<T> &points, size_t subsetCount, const EigenSolverFunc &eigenSolver)
{
	const size_t n = points.rows();
	const size_t dimension = points.cols();
    std::cout << "Initializing blocked PCA, " << n << " points, " << dimension << " total dims, " << subsetCount << " subsets" << std::endl;
	
	_subsets.resize(subsetCount);

	int allocatedDims = 0;
	int subsetIndex = 0;
	while (allocatedDims < dimension)
	{
		_subsets[subsetIndex].dimCount++;
		allocatedDims++;
		subsetIndex = (subsetIndex + 1) % _subsetCount;
	}

	int startDim = 0;
	for (int i = 0; i < subsetCount; i++)
	{
		_subsets[i].startDim = startDim;
		startDim += _subsets[i].dimCount;
	}

	for (Subset &s : _subsets)
	{
		DenseMatrix<T> subpoints(n, s.dimCount);
		for (int p = 0; p < n; p++)
			for (int d = 0; d < s.dimCount; d++)
				subpoints(p, d) = points(p, d + s.startDim);
		s.pca.init(subpoints, eigenSolver);
	}
	
}

template<class T>
void BlockedPCA<T>::transform(const T *input, size_t reducedSubsetDimension, T *result) const
{
	for (int i = 0; i < reducedSubsetDimension * subset; i++)
		result[i] = 0;

	for (Subset &s : _subsets)
	{
		const int reducedDim = std::min(reducedSubsetDimension, s.dimCount);
		s.transform(input + s.startDim, reducedDim, result + s.startDim);
	}
}

template<class T>
void BlockedPCA<T>::inverseTransform(const T *input, size_t reducedSubsetDimension, T *result) const
{
	for (int i = 0; i < reducedSubsetDimension * subset; i++)
		result[i] = 0;

	for (Subset &s : _subsets)
	{
		const int reducedDim = std::min(reducedSubsetDimension, s.dimCount);
		s.inverseTransform(input + s.startDim, reducedDim, result + s.startDim);
	}
}

template<class T>
void BlockedPCA<T>::save(const std::string &baseFilename) const
{
    BinaryDataStreamFile file(baseFilename + ".dat", true);
	size_t subsetCount = _subsets.size();
    file << subsetCount;
	for (int i = 0; i < subsetCount; i++)
	{
		file << _subsets[i].startDim << _subsets[i].dimCount;
		_subsets[i].pca.save(baseFilename + "_" + to_string(i) + ".dat");
	}
    file.closeStream();


}

template<class T>
void BlockedPCA<T>::load(const std::string &baseFilename)
{
    BinaryDataStreamFile file(baseFilename + ".dat", false);
	size_t subsetCount;
	file >> subsetCount;
	_subsets.resize(subsetCount);
	for (int i = 0; i < subsetCount; i++)
	{
		file >> _subsets[i].startDim >> _subsets[i].dimCount;
		_subsets[i].pca.load(baseFilename + "_" + to_string(i) + ".dat");
	}
    //file.closeStream();
}
