
#ifndef CORE_BASE_GRID3D_INL_H_
#define CORE_BASE_GRID3D_INL_H_

namespace ml
{

	template <class T> Grid3<T>::Grid3()
	{
		m_dimX = 0;
		m_dimY = 0;
		m_dimZ = 0;
		m_data = nullptr;
	}

	template <class T> Grid3<T>::Grid3(size_t dimX, size_t dimY, size_t dimZ)
	{
		m_dimX = dimX;
		m_dimY = dimY;
		m_dimZ = dimZ;
		m_data = new T[dimX * dimY * dimZ];
	}

	template <class T> Grid3<T>::Grid3(size_t dimX, size_t dimY, size_t dimZ, const T &clearValue)
	{
		m_dimX = dimX;
		m_dimY = dimY;
		m_dimZ = dimZ;
		m_data = new T[dimX * dimY * dimZ];
		setValues(clearValue);
	}

	template <class T> Grid3<T>::Grid3(const Grid3<T> &G)
	{
		m_dimX = G.m_dimX;
		m_dimY = G.m_dimY;
		m_dimZ = G.m_dimZ;

		const size_t totalEntries = getNumEntries();
		m_data = new T[totalEntries];
		for (size_t i = 0; i < totalEntries; i++) {
			m_data[i] = G.m_data[i];
		}
	}

	template <class T> Grid3<T>::Grid3(Grid3<T> &&G)
	{
		m_dimX = G.m_dimX;
		m_dimY = G.m_dimY;
		m_dimZ = G.m_dimZ;

		m_data = G.m_data;

		G.m_dimX = 0;
		G.m_dimY = 0;
		G.m_dimZ = 0;

		G.m_data = nullptr;
	}

	template <class T> Grid3<T>::Grid3(size_t dimX, size_t dimY, size_t dimZ, const std::function< T(size_t, size_t, size_t) > &fillFunction)
	{
		m_dimX = dimX;
		m_dimY = dimY;
		m_dimY = dimZ;
		m_data = new T[dimX * dimY * dimZ];
		fill(fillFunction);
	}

	template <class T> Grid3<T>::~Grid3()
	{
		free();
	}

	template <class T> void Grid3<T>::free()
	{
		m_dimX = 0;
		m_dimY = 0;
		m_dimZ = 0;
		if (m_data != nullptr)
		{
			delete[] m_data;
			m_data = nullptr;
		}
	}

	template <class T> Grid3<T>& Grid3<T>::operator = (const Grid3<T> &G)
	{
		if (m_data) delete[] m_data;
		m_dimX = G.m_dimX;
		m_dimY = G.m_dimY;
		m_dimZ = G.m_dimZ;

		const size_t totalEntries = getNumEntries();
		m_data = new T[totalEntries];
		for (size_t i = 0; i < totalEntries; i++) {
			m_data[i] = G.m_data[i];
		}

		return *this;
	}

	template <class T> Grid3<T>& Grid3<T>::operator = (Grid3<T> &&G)
	{
		std::swap(m_dimX, G.m_dimX);
		std::swap(m_dimY, G.m_dimY);
		std::swap(m_dimZ, G.m_dimZ);
		std::swap(m_data, G.m_data);
		return *this;
	}

	template <class T> void Grid3<T>::allocate(size_t dimX, size_t dimY, size_t dimZ)
	{
		if (dimX == 0 || dimY == 0 || dimZ == 0) {
			free();
		}
		else {
			m_dimX = dimX;
			m_dimY = dimY;
			m_dimZ = dimZ;
			if (m_data) delete[] m_data;
			m_data = new T[dimX * dimY * dimZ];
		}
	}

	template <class T> void Grid3<T>::allocate(size_t dimX, size_t dimY, size_t dimZ, const T &clearValue)
	{
		if (dimX == 0 || dimY == 0 || dimZ == 0) {
			free();
		}
		else {
			allocate(dimX, dimY, dimZ);
			setValues(clearValue);
		}
	}

	template <class T> void Grid3<T>::setValues(const T &value)
	{
		const size_t totalEntries = getNumEntries();
		for (size_t i = 0; i < totalEntries; i++) m_data[i] = value;
	}

	template <class T> vec3ul Grid3<T>::getMaxIndex() const
	{
		vec3ul maxIndex(0, 0, 0);
		const T *maxValue = m_data;
		for (size_t z = 0; z < m_dimZ; z++)
			for (size_t y = 0; y < m_dimY; y++)
				for (size_t x = 0; x < m_dimX; x++)
				{
					const T *curValue = &m_data[z * m_dimY * m_dimX + y * m_dimX + x];
					if (*curValue > *maxValue)
					{
						maxIndex = vec3ul(x, y, z);
						maxValue = curValue;
					}
				}
		return maxIndex;
	}

	template <class T> const T& Grid3<T>::getMaxValue() const
	{
		vec3ul index = getMaxIndex();
		return m_data[index.z * m_dimY * m_dimX + index.y * m_dimX + index.x];
	}

	template <class T> vec3ul Grid3<T>::getMinIndex() const
	{
		vec3ul minIndex(0, 0, 0);
		const T *minValue = &m_data[0];
		for (size_t z = 0; z < m_dimZ; z++)
			for (size_t y = 0; y < m_dimY; y++)
				for (size_t x = 0; x < m_dimX; x++)
				{
					const T *curValue = &m_data[z * m_dimX * m_dimY + y * m_dimX + x];
					if (*curValue < *minValue)
					{
						minIndex = vec3ul(x, y, z);
						minValue = curValue;
					}
				}
			return minIndex;
	}

	template <class T> const T& Grid3<T>::getMinValue() const
	{
		vec3ul index = getMinIndex();
		return m_data[z * m_dimX * m_dimY + y * m_dimX + x];
	}

}  // namespace ml

#endif  // CORE_BASE_GRID3D_INL_H_