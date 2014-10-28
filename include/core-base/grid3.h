
#ifndef CORE_BASE_GRID3D_H_
#define CORE_BASE_GRID3D_H_

namespace ml
{

template <class T> class Grid3
{
public:
	Grid3();
	Grid3(size_t rows, size_t cols, size_t slices);
	Grid3(size_t rows, size_t cols, size_t slices, const T &clearValue);
	Grid3(const Grid3<T> &G);
	Grid3(Grid3<T> &&G);

	~Grid3();

	//
	// Memory
	//
	void deleteMemory();
	Grid3<T>& operator = (const Grid3<T> &G);
	Grid3<T>& operator = (Grid3<T> &&G);

	void allocate(size_t rows, size_t cols, size_t slices);
	void allocate(size_t rows, size_t cols, size_t slices, const T &clearValue);

	inline Grid3<T>& operator += (const Grid3<T> &right)
	{
		MLIB_ASSERT_STR(m_rows == right.m_rows && m_cols == right.m_cols, "grid dimensions must be equal");
		for (size_t i = 0; i < getNumTotalEntries(); i++) {
			m_data[i] += right.m_data[i];
		}
		return *this;
	}
	inline Grid3<T>& operator *= (T right)
	{
		for (size_t i = 0; i < getNumTotalEntries(); i++) {
			m_data[i] *= right.m_data[i];
		}
		return *this;
	}

	inline Grid3<T> operator * (T x)
	{
		Grid3<T> result(m_rows, m_cols, m_slices);
		for (size_t i = 0; i < getNumTotalEntries(); i++) {
			result.m_data =  m_data * x;
		}
		return result;
	}

	//
	// Accessors
	//
	inline T& operator() (size_t row, size_t col, size_t slice)
	{
#if defined(MLIB_BOUNDS_CHECK) || defined(_DEBUG)
		MLIB_ASSERT_STR( (row < m_rows) && (col < m_cols) && (slice < m_slices), "Out-of-bounds grid access");
#endif
		return m_data[slice*m_cols*m_rows + row*m_cols + col];
	}
	inline const T& operator() (size_t row, size_t col, size_t slice) const
	{
#if defined(MLIB_BOUNDS_CHECK) || defined(_DEBUG)
		MLIB_ASSERT_STR( (row < m_rows) && (col < m_cols) && (slice < m_slices), "Out-of-bounds grid access");
#endif
		return m_data[slice*m_cols*m_rows + row*m_cols + col];
	}
	inline size_t dimX() const
	{
		return m_rows;
	}
	inline size_t dimY() const
	{
		return m_cols;
	}
	inline size_t dimZ() const 
	{
		return m_slices;
	}

	//inline vec3ui getDimensions() const {
	//	return vec3ui(dimX(), dimY(), dimZ());
	//}

	inline bool square() const
	{
		return (m_rows == m_cols && m_cols == m_slices);
	}
	inline T* ptr()
	{
		return m_data;
	}
	inline const T* ptr() const
	{
		return m_data;
	}

	//
	// Query
	//
	inline bool validCoordinates(size_t row, size_t col, size_t slices ) const
	{
		return (row < m_rows && col < m_cols && slices < m_slices);
	}

	//
	// Modifiers
	//
	void clear(const T &clearValue);

	size_t getNumTotalEntries() const {
		return m_rows * m_cols * m_slices;
	}
protected:
	T *m_data;
	size_t m_rows, m_cols, m_slices;
};

template <class T> inline bool operator == (const Grid3<T> &a, const Grid3<T> &b)
{
	if(a.rows() != b.rows() || a.cols() != b.cols() || a.slices() != b.slices()) return false;
	const size_t totalEntries = a.getNumTotalEntries();
	for (size_t i = 0; i < totalEntries; i++) {
		if (a.ptr()[i] != b.ptr()[i])	return false;
	}
	return true;
}

template <class T> inline bool operator != (const Grid3<T> &a, const Grid3<T> &b)
{
	return !(a == b);
}

}  // namespace ml

#include "grid3.cpp"

#endif  // CORE_BASE_GRID3D_H_
