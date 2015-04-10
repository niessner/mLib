
#ifndef CORE_BASE_GRID2D_H_INL_
#define CORE_BASE_GRID2D_H_INL_

namespace ml
{

	template <class T> Grid2<T>::Grid2()
	{
		m_dimX = 0;
		m_dimY = 0;
		m_data = nullptr;
	}

	template <class T> Grid2<T>::Grid2(size_t dimX, size_t dimY)
	{
		m_dimX = dimX;
		m_dimY = dimY;
		m_data = new T[dimX * dimY];
	}

	template <class T> Grid2<T>::Grid2(size_t dimX, size_t dimY, const T &value)
	{
		m_dimX = dimX;
		m_dimY = dimY;
		m_data = new T[dimX * dimY];
		setValues(value);
	}

	template <class T> Grid2<T>::Grid2(const Grid2<T> &G)
	{
		m_dimX = G.m_dimX;
		m_dimY = G.m_dimY;

		const size_t totalEntries = m_dimX * m_dimY;
		m_data = new T[totalEntries];
		for (size_t i = 0; i < totalEntries; i++)
			m_data[i] = G.m_data[i];
	}

	template <class T> Grid2<T>::Grid2(Grid2<T> &&G)
	{
		m_dimX = G.m_dimX;
		m_dimY = G.m_dimY;
		m_data = G.m_data;

		G.m_dimX = 0;
		G.m_dimY = 0;
		G.m_data = nullptr;
	}

	template <class T> Grid2<T>::Grid2(size_t dimX, size_t dimY, const std::function< T(size_t, size_t) > &fillFunction)
	{
		m_dimX = dimX;
		m_dimY = dimY;
		m_data = new T[dimX * dimY];
		fill(fillFunction);
	}

	template <class T> Grid2<T>::~Grid2()
	{
		free();
	}

	template <class T> void Grid2<T>::free()
	{
		m_dimX = 0;
		m_dimY = 0;
		SAFE_DELETE(m_data);
	}

	template <class T> Grid2<T>& Grid2<T>::operator = (const Grid2<T> &G)
	{
		if (m_data) delete[] m_data;
		m_dimX = G.m_dimX;
		m_dimY = G.m_dimY;

		const size_t totalEntries = m_dimX * m_dimY;
		m_data = new T[totalEntries];
		for (size_t i = 0; i < totalEntries; i++)
			m_data[i] = G.m_data[i];

		return *this;
	}

	template <class T> Grid2<T>& Grid2<T>::operator = (Grid2<T> &&G)
	{
		std::swap(m_dimX, G.m_dimX);
		std::swap(m_dimY, G.m_dimY);
		std::swap(m_data, G.m_data);
		return *this;
	}

	template <class T> void Grid2<T>::allocate(size_t dimX, size_t dimY)
	{
		if (dimX == 0 || dimY == 0) {
			free();
		}
		else {
			m_dimX = dimX;
			m_dimY = dimY;
			if (m_data) delete[] m_data;
			m_data = new T[dimX * dimY];
		}
	}

	template <class T> void Grid2<T>::allocate(size_t dimX, size_t dimY, const T &value)
	{
		if (dimX == 0 || dimY == 0) {
			free();
		} else {
			allocate(dimX, dimY);
			setValues(value);
		}
	}

	template <class T> void Grid2<T>::setValues(const T &value)
	{
		const size_t totalEntries = m_dimX * m_dimY;
		for (size_t i = 0; i < totalEntries; i++)
			m_data[i] = value;
	}

	template <class T> void Grid2<T>::fill(const std::function< T(size_t, size_t) > &fillFunction)
	{
		for (size_t y = 0; y < m_dimY; y++)
			for (size_t x = 0; x < m_dimX; x++)
			{
				m_data[y * m_dimX + x] = fillFunction(x, y);
			}
	}

	template <class T> vec2ul Grid2<T>::getMaxIndex() const
	{
		vec2ul maxIndex(0, 0);
		const T *maxValue = m_data;
		for (size_t y = 0; y < m_dimY; y++)
			for (size_t x = 0; x < m_dimX; x++)
			{
				const T *curValue = &m_data[y * m_dimX + x];
				if (*curValue > *maxValue)
				{
					maxIndex = vec2ul(x, y);
					maxValue = curValue;
				}
			}
		return maxIndex;
	}

	template <class T> const T& Grid2<T>::getMaxValue() const
	{
		vec2ul index = getMaxIndex();
		return m_data[index.y * m_dimX + index.x];
	}

	template <class T> vec2ul Grid2<T>::getMinIndex() const
	{
		vec2ul minIndex(0, 0);
		const T *minValue = &m_data[0];
		for (size_t y = 0; y < m_dimY; y++)
			for (size_t x = 0; x < m_dimX; x++)
			{
				const T *curValue = &m_data[y * m_dimX + x];
				if (*curValue < *minValue)
				{
					minIndex = vec2ul(x, y);
					minValue = curValue;
				}
			}
		return minIndex;
	}

	template <class T> const T& Grid2<T>::getMinValue() const
	{
		vec2ul index = getMinIndex();
        return m_data[index.y * m_dimX + index.x];
	}

}  // namespace ml

#endif  // CORE_BASE_GRID2D_H_INL_
