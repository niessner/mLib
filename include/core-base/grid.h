/*
Grid.h
Written by Matthew Fisher

Arbitrary-T grid structure.
*/

#pragma once

template <class T> class Grid
{
public:
	Grid();
	Grid(UINT rows, UINT cols);
	Grid(UINT rows, UINT cols, const T &clearValue);
	Grid(const Grid<T> &G);
	Grid(Grid<T> &&G);

	~Grid();

	//
	// Memory
	//
	void deleteMemory();
	Grid<T>& operator = (const Grid<T> &G);
	Grid<T>& operator = (Grid<T> &&G);

	void allocate(UINT rows, UINT cols);
	void allocate(UINT rows, UINT cols, const T &clearValue);

	inline Grid<T>& Grid<T>::operator += (const Grid<T> &right)
	{
		MLIB_ASSERT(m_rows == right.m_rows && m_cols == right.m_cols, "grid dimensions must be equal");
		for (UINT r = 0; r < m_rows; r++)
			for (UINT c = 0; c < m_cols; c++)
				m_data[r * m_cols + c] += right(r,c);
		return *this;
	}
	inline Grid<T>& Grid<T>::operator *= (T right)
	{
		for (UINT r = 0; r < m_rows; r++)
			for (UINT c = 0; c < m_cols; c++)
				m_data[r * m_cols + c] *= right;
		return *this;
	}

	inline Grid<T> operator * (T x)
	{
		Grid<T> result(m_rows, m_cols);
		for (UINT r = 0; r < m_rows; r++)
			for (UINT c = 0; c < m_cols; c++)
				result(r,c) = m_data[r * m_cols + c] * x;
		return result;
	}

	//
	// Accessors
	//
	inline T& operator() (UINT row, UINT col)
	{
#if defined(MLIB_BOUNDS_CHECK) || defined(DEBUG)
		MLIB_ASSERT( (row < m_rows) && (col < m_cols), "Out-of-bounds grid access");
#endif
		return m_data[row * m_cols + col];
	}
	inline const T& operator() (UINT row, UINT col) const
	{
#if defined(MLIB_BOUNDS_CHECK) || defined(DEBUG)
		MLIB_ASSERT( (row < m_rows) && (col < m_cols), "Out-of-bounds grid access");
#endif
		return m_data[row * m_cols + col];
	}
	inline UINT rows() const
	{
		return m_rows;
	}
	inline UINT cols() const
	{
		return m_cols;
	}
	inline std::pair<UINT, UINT> dimensions() const
	{
		return std::make_pair(m_rows, m_cols);
	}
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
	inline bool validCoordinates(int row, int col) const
	{
		return (row >= 0 && row < int(m_rows) && col >= 0 && col < int(m_cols));
	}

	void setRow(UINT row, const Vector<T> &values)
	{
		for(UINT col = 0; col < m_cols; col++) m_data[row * m_cols + col] = values[col];
	}

	void setCol(UINT col, const Vector<T> &values)
	{
		for(UINT row = 0; row < m_rows; row++) m_data[row * m_cols + col] = values[row];
	}

	Vector<T> getRow(UINT row) const
	{
		Vector<T> result;
		const T *CPtr = m_data;
		result.allocate(m_cols);
		for(UINT col = 0; col < m_cols; col++)
		{
			result[col] = CPtr[row * m_cols + col];
		}
		return result;
	}

	Vector<T> getCol(UINT col) const
	{
		Vector<T> result;
		const T *CPtr = m_data;
		result.allocate(m_rows);
		for(UINT row = 0; row < m_rows; row++)
		{
			result[col] = CPtr[row * m_cols + col];
		}
		return result;
	}

	std::pair<UINT, UINT> maxIndex() const;
	const T& maxValue() const;
	std::pair<UINT, UINT> minIndex() const;
	const T& minValue() const;

	//
	// Modifiers
	//
	void clear(const T &T);

protected:
	T *m_data;
	UINT m_rows, m_cols;
};

#include "../../src/core-base/grid.cpp"