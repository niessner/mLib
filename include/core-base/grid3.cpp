
#ifndef CORE_BASE_GRID3D_INL_H_
#define CORE_BASE_GRID3D_INL_H_

namespace ml
{

template <class T> Grid3<T>::Grid3()
{
	m_width = 0;
	m_height = 0;
	m_depth = 0;
	m_data = nullptr;
}

template <class T> Grid3<T>::Grid3(size_t rows, size_t cols, size_t slices)
{
	m_width = rows;
	m_height = cols;
	m_depth = slices;
	m_data = new T[rows * cols * slices];
}

template <class T> Grid3<T>::Grid3(size_t rows, size_t cols, size_t slices, const T &clearValue)
{
	m_width = rows;
	m_height = cols;
	m_depth = slices;
	m_data = new T[rows * cols * slices];
	clear(clearValue);
}

template <class T> Grid3<T>::Grid3(const Grid3<T> &G)
{
	m_width = G.m_rows;
	m_height = G.m_cols;
	m_depth = G.m_slices;

	const size_t totalEntries = getNumTotalEntries();
	m_data = new T[totalEntries];
	for(size_t i = 0; i < totalEntries; i++) {
		m_data[i] = G.m_data[i];
	}
}

template <class T> Grid3<T>::Grid3(Grid3<T> &&G)
{
	m_height = G.m_cols;
	m_width = G.m_rows;
	m_depth = G.m_slices;

	m_data = G.m_data;

	G.m_rows = 0;
	G.m_cols = 0;
	G.m_slices = 0;

	G.m_data = nullptr;
}

template <class T> Grid3<T>::~Grid3()
{
	deleteMemory();
}

template <class T> void Grid3<T>::deleteMemory()
{
	m_width = 0;
	m_height = 0;
	m_depth = 0;
	if(m_data != nullptr)
	{
		delete[] m_data;
		m_data = nullptr;
	}
}

template <class T> Grid3<T>& Grid3<T>::operator = (const Grid3<T> &G)
{
	if(m_data) delete[] m_data;
	m_width = G.m_rows;
	m_height = G.m_cols;
	m_depth = G.m_slices;

	const size_t totalEntries = getNumTotalEntries();
	m_data = new T[totalEntries];
	for (size_t i = 0; i < totalEntries; i++) {
		m_data[i] = G.m_data[i];
	}

	return *this;
}

template <class T> Grid3<T>& Grid3<T>::operator = (Grid3<T> &&G)
{
	std::swap(m_width, G.m_rows);
	std::swap(m_height, G.m_cols);
	std::swap(m_depth, G.m_slices);
	std::swap(m_data, G.m_data);
	return *this;
}

template <class T> void Grid3<T>::allocate(size_t rows, size_t cols, size_t slices)
{
	m_width = rows;
	m_height = cols;
	m_depth = slices;
	if(m_data) delete[] m_data;
	m_data = new T[rows * cols * slices];
}

template <class T> void Grid3<T>::allocate(size_t rows, size_t cols, size_t slices, const T &clearValue)
{
	allocate(rows, cols, slices);
	clear(clearValue);
}

template <class T> void Grid3<T>::clear(const T &clearValue)
{
	const size_t totalEntries = getNumTotalEntries();
	for(size_t i = 0; i < totalEntries; i++) m_data[i] = clearValue;
}

}  // namespace ml

#endif  // CORE_BASE_GRID3D_INL_H_