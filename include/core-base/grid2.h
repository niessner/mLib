
#ifndef CORE_BASE_GRID2D_H_
#define CORE_BASE_GRID2D_H_

namespace ml
{

template <class T> class Grid2
{
public:
	Grid2();
	Grid2(size_t rows, size_t cols);
	Grid2(size_t rows, size_t cols, const T &clearValue);
	Grid2(const Grid2<T> &G);
	Grid2(Grid2<T> &&G);

	~Grid2();

	//
	// Memory
	//
	void deleteMemory();
	Grid2<T>& operator = (const Grid2<T> &G);
	Grid2<T>& operator = (Grid2<T> &&G);

	void allocate(size_t rows, size_t cols);
	void allocate(size_t rows, size_t cols, const T &clearValue);

	inline Grid2<T>& operator += (const Grid2<T> &right)
	{
		MLIB_ASSERT_STR(m_rows == right.m_rows && m_cols == right.m_cols, "grid dimensions must be equal");
		for (size_t r = 0; r < m_rows; r++)
			for (size_t c = 0; c < m_cols; c++)
				m_data[r * m_cols + c] += right(r,c);
		return *this;
	}
	inline Grid2<T>& operator *= (T right)
	{
		for (size_t r = 0; r < m_rows; r++)
			for (size_t c = 0; c < m_cols; c++)
				m_data[r * m_cols + c] *= right;
		return *this;
	}

	inline Grid2<T> operator * (T x)
	{
		Grid2<T> result(m_rows, m_cols);
		for (size_t r = 0; r < m_rows; r++)
			for (size_t c = 0; c < m_cols; c++)
				result(r,c) = m_data[r * m_cols + c] * x;
		return result;
	}

	//
	// Accessors
	//
	inline T& operator() (size_t row, size_t col)
	{
#if defined(MLIB_BOUNDS_CHECK) || defined(_DEBUG)
		MLIB_ASSERT_STR( (row < m_rows) && (col < m_cols), "Out-of-bounds grid access");
#endif
		return m_data[row * m_cols + col];
	}
	inline const T& operator() (size_t row, size_t col) const
	{
#if defined(MLIB_BOUNDS_CHECK) || defined(_DEBUG)
		MLIB_ASSERT_STR( (row < m_rows) && (col < m_cols), "Out-of-bounds grid access");
#endif
		return m_data[row * m_cols + col];
	}
	inline size_t rows() const
	{
		return m_rows;
	}
	inline size_t cols() const
	{
		return m_cols;
	}

	inline size_t dimX() const {
		return m_rows;
	}

	inline size_t dimY() const {
		return m_cols;
	}

	inline std::pair<size_t, size_t> dimensions() const
	{
		return std::make_pair(m_rows, m_cols);
	}

	//inline vec2ul getDimensions() const {
	//	return vec2ul(m_rows, m_cols);
	//}

	inline bool square() const
	{
		return (m_rows == m_cols);
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
	inline bool isValidCoordinate(int row, int col) const
	{
		return (row >= 0 && row < int(m_rows) && col >= 0 && col < int(m_cols));
	}

	void setRow(size_t row, const std::vector<T> &values)
	{
		for(size_t col = 0; col < m_cols; col++) m_data[row * m_cols + col] = values[col];
	}

	void setCol(size_t col, const std::vector<T> &values)
	{
		for(size_t row = 0; row < m_rows; row++) m_data[row * m_cols + col] = values[row];
	}

	std::vector<T> getRow(size_t row) const
	{
		std::vector<T> result(m_cols);
		const T *CPtr = m_data;
		for(size_t col = 0; col < m_cols; col++)
		{
			result[col] = CPtr[row * m_cols + col];
		}
		return result;
	}

	std::vector<T> getCol(size_t col) const
	{
		std::vector<T> result(m_rows);
		const T *CPtr = m_data;
		for(size_t row = 0; row < m_rows; row++)
		{
			result[row] = CPtr[row * m_cols + col];
		}
		return result;
	}

	std::pair<size_t, size_t> maxIndex() const;
	const T& maxValue() const;
	std::pair<size_t, size_t> minIndex() const;
	const T& minValue() const;

	//
	// Modifiers
	//
	void clear(const T &clearValue);

    const T* begin() const
    {
        return m_data;
    }
    T* begin()
    {
        return m_data;
    }
    const T* end() const
    {
        return m_data + m_rows * m_cols;
    }
    T* end()
    {
        return m_data + m_rows * m_cols;
    }

protected:
	T *m_data;
	size_t m_rows, m_cols;
};

template <class T> inline bool operator == (const Grid2<T> &a, const Grid2<T> &b)
{
	if(a.rows() != b.rows() || a.cols() != b.cols()) return false;
	for(size_t row = 0; row < a.rows(); row++)
		for(size_t col = 0; col < a.cols(); col++)
			if(a(row, col) != b(row, col))
				return false;
	return true;
}

template <class T> inline bool operator != (const Grid2<T> &a, const Grid2<T> &b)
{
	return !(a == b);
}

typedef Grid2<float> Grid2f;
typedef Grid2<double> Grid2d;

}

#include "grid2.cpp"

#endif  // CORE_BASE_GRID2D_H_
