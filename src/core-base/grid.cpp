
template <class T> Grid<T>::Grid()
{
	m_rows = 0;
	m_cols = 0;
	m_data = NULL;
}

template <class T> Grid<T>::Grid(UINT rows, UINT cols)
{
	m_rows = rows;
	m_cols = cols;
	m_data = new T[rows * cols];
}

template <class T> Grid<T>::Grid(UINT rows, UINT cols, const T &clearValue)
{
	m_rows = rows;
	m_cols = cols;
	m_data = new T[rows * cols];
	clear(clearValue);
}

template <class T> Grid<T>::Grid(const Grid<T> &G)
{
	m_rows = G.m_rows;
	m_cols = G.m_cols;

	const UINT totalEntries = m_rows * m_cols;
	m_data = new T[totalEntries];
	for(UINT i = 0; i < totalEntries; i++)
	{
		m_data[i] = G.m_data[i];
	}
}

template <class T> Grid<T>::Grid(Grid<T> &&G)
{
	m_rows = G.m_rows;
	m_cols = G.m_cols;
	m_data = G.m_data;

	G.m_rows = 0;
	G.m_cols = 0;
	G.m_data = NULL;
}

template <class T> Grid<T>::~Grid()
{
	deleteMemory();
}

template <class T> void Grid<T>::deleteMemory()
{
	m_rows = 0;
	m_cols = 0;
	if(m_data != NULL)
	{
		delete[] m_data;
		m_data = NULL;
	}
}

template <class T> Grid<T>& Grid<T>::operator = (const Grid<T> &G)
{
	if(m_data) delete[] m_data;
	m_rows = G.m_rows;
	m_cols = G.m_cols;

	const UINT totalEntries = m_rows * m_cols;
	m_data = new T[totalEntries];
	for(UINT i = 0; i < totalEntries; i++) m_data[i] = G.m_data[i];

	return *this;
}

template <class T> Grid<T>& Grid<T>::operator = (Grid<T> &&G)
{
	if(&G != this)
	{
		deleteMemory();
		m_data = G.m_data;
		m_rows = G.m_rows;
		m_cols = G.m_cols;
	}
	return *this;
}

template <class T> void Grid<T>::allocate(UINT rows, UINT cols)
{
	m_rows = rows;
	m_cols = cols;
	if(m_data) delete[] m_data;
	m_data = new T[rows * cols];
}

template <class T> void Grid<T>::allocate(UINT rows, UINT cols, const T &clearValue)
{
	allocate(rows, cols);
	clear(clearValue);
}

template <class T> void Grid<T>::clear(const T &clearValue)
{
	const UINT totalEntries = m_rows * m_cols;
	for(UINT i = 0; i < totalEntries; i++) m_data[i] = clearValue;
}

template <class T> std::pair<UINT, UINT> Grid<T>::maxIndex() const
{
	std::pair<UINT, UINT> maxIndex(0, 0);
	const T *maxValue = m_data;
	for(UINT rowIndex = 0; rowIndex < m_rows; rowIndex++)
		for(UINT colIndex = 0; colIndex < m_cols; colIndex++)
		{
			const T *curValue = &m_data[rowIndex * m_cols + colIndex];
			if(*curValue > *maxValue)
			{
				maxIndex = std::make_pair(rowIndex, colIndex);
				maxValue = curValue;
			}
		}
	return maxIndex;
}

template <class T> const T& Grid<T>::maxValue() const
{
	std::pair<UINT, UINT> index = maxIndex();
	return m_data[index.first * m_cols + index.second];
}

template <class T> std::pair<UINT, UINT> Grid<T>::minIndex() const
{
	std::pair<UINT, UINT> minIndex(0, 0);
	const T *minValue = &m_data[0];
	for(UINT rowIndex = 0; rowIndex < m_rows; rowIndex++)
	{
		for(UINT colIndex = 0; colIndex < m_cols; colIndex++)
		{
			const T *curValue = &m_data[rowIndex * m_cols + colIndex];
			if(*curValue < *minValue)
			{
				minIndex = std::make_pair(rowIndex, colIndex);
				minValue = curValue;
			}
		}
	}
	return minIndex;
}

template <class T> const T& Grid<T>::minValue() const
{
	std::pair<UINT, UINT> index = minIndex();
	return m_data[index.first * m_cols + index.second];
}
