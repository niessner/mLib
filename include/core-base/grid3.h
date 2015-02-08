
#ifndef CORE_BASE_GRID3D_H_
#define CORE_BASE_GRID3D_H_

namespace ml
{

template <class T> class Grid3
{
public:
	Grid3();
	Grid3(size_t width, size_t height, size_t depth);
	Grid3(size_t width, size_t height, size_t depth, const T &clearValue);
	Grid3(const Grid3<T> &G);
	Grid3(Grid3<T> &&G);

	~Grid3();

	//
	// Memory
	//
	void deleteMemory();
	Grid3<T>& operator = (const Grid3<T> &G);
	Grid3<T>& operator = (Grid3<T> &&G);

	void allocate(size_t width, size_t height, size_t depth);
	void allocate(size_t width, size_t height, size_t depth, const T &clearValue);

	inline Grid3<T>& operator += (const Grid3<T> &right)
	{
		MLIB_ASSERT_STR(m_width == right.m_rows && m_height == right.m_cols, "grid dimensions must be equal");
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
		Grid3<T> result(m_width, m_height, m_depth);
		for (size_t i = 0; i < getNumTotalEntries(); i++) {
			result.m_data =  m_data * x;
		}
		return result;
	}

	//
	// Accessors
	//
	inline T& operator() (size_t x, size_t y, size_t z)
	{
#if defined(MLIB_BOUNDS_CHECK) || defined(_DEBUG)
		MLIB_ASSERT_STR( (x < m_width) && (y < m_height) && (z < m_depth), "Out-of-bounds grid access");
#endif
		return m_data[z*m_height*m_width + x*m_height + y];
	}
	inline const T& operator() (size_t row, size_t col, size_t slice) const
	{
#if defined(MLIB_BOUNDS_CHECK) || defined(_DEBUG)
		MLIB_ASSERT_STR( (row < m_width) && (col < m_height) && (slice < m_depth), "Out-of-bounds grid access");
#endif
		return m_data[slice*m_height*m_width + row*m_height + col];
	}
	inline size_t dimX() const
	{
		return m_width;
	}
	inline size_t dimY() const
	{
		return m_height;
	}
	inline size_t dimZ() const 
	{
		return m_depth;
	}

	//inline vec3ul getDimensions() const {
	//	return vec3ul(dimX(), dimY(), dimZ());
	//}

	inline bool square() const
	{
		return (m_width == m_height && m_height == m_depth);
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
	inline bool isValidCoordinate(size_t x, size_t y, size_t z ) const
	{
		return (x < m_width && y < m_height && z < m_depth);
	}

	//
	// Modifiers
	//
	void clear(const T &clearValue);

	size_t getNumTotalEntries() const {
		return m_width * m_height * m_depth;
	}
protected:
	T *m_data;
	size_t m_width, m_height, m_depth;
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

typedef Grid3<float> Grid3f;
typedef Grid3<double> Grid3d;

}  // namespace ml

#include "grid3.cpp"

#endif  // CORE_BASE_GRID3D_H_
