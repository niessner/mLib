
#pragma once
//#pragma warning(disable : 4700)

template <class T> void Vector<T>::clear(const T &clearValue)
{
    const UINT64 length = m_length;
    T *ptr = m_data;
    for(UINT64 i = 0; i < length; i++) ptr[i] = clearValue;
}

template <class T> T Vector<T>::sum() const
{
    const UINT64 length = m_length;
    if(m_length == 0)
    {
        T result;
		memset(&result, 0, sizeof(T));
        return result;
    }
    else
    {
        const T *ptr = m_data;
        T result = ptr[0];
        for(UINT64 i = 1; i < length; i++) result += ptr[i];
        return result;
    }
}

template <class T> T Vector<T>::product() const
{
    const UINT64 length = m_length;
    if(m_length == 0)
    {
        T result;
        memset(&result, 0, sizeof(T));
        return result;
    }
    else
    {
        const T *ptr = m_data;
        T result = ptr[0];
        for(UINT64 i = 1; i < length; i++) result *= ptr[i];
        return result;
    }
}

template <class T> UINT64 Vector<T>::minIndex() const
{
	MLIB_ASSERT_STR(m_length > 0, "minIndex called on zero-length vector");
	const UINT64 length = m_length;
	const T *ptr = m_data;
	UINT64 smallestIndexSeen = 0;
	for(UINT64 i = 1; i < length; i++)
		if(ptr[i] < ptr[smallestIndexSeen])
			smallestIndexSeen = i;
	return smallestIndexSeen;
}

template <class T> UINT64 Vector<T>::maxIndex() const
{
    MLIB_ASSERT_STR(m_length > 0, "maxIndex called on zero-length vector");
    const UINT64 length = m_length;
    const T *ptr = m_data;
    UINT64 largestIndexSeen = 0;
    for(UINT64 i = 1; i < length; i++)
        if(ptr[i] > ptr[largestIndexSeen])
            largestIndexSeen = i;
    return largestIndexSeen;
}

template <class T> const T& Vector<T>::minValue() const
{
	return m_data[minIndex()];
}

template <class T> const T& Vector<T>::maxValue() const
{
    return m_data[maxIndex()];
}

template <class T> UINT64 Vector<T>::maxIndex(const std::function<double(const T&)> &valueFunction) const
{
	MLIB_ASSERT_STR(m_length > 0, "maxIndex called on zero-length vector");
	double maxValue = valueFunction(m_data[0]);
	UINT64 maxIndex = 0;
	for(UINT64 i = 0; i < m_length; i++)
	{
		double value = valueFunction(m_data[i]);
		if(value > maxValue)
		{
			maxValue = value;
			maxIndex = i;
		}
	}
	return maxIndex;
}
template <class T> UINT64 Vector<T>::minIndex(const std::function<double(const T&)> &valueFunction) const
{
	MLIB_ASSERT_STR(m_length > 0, "minIndex called on zero-length vector");
	double minValue = valueFunction(m_data[0]);
	UINT64 minIndex = 0;
	for(UINT64 i = 0; i < m_length; i++)
	{
		double value = valueFunction(m_data[i]);
		if(value < minValue)
		{
			minValue = value;
			minIndex = i;
		}
	}
	return minIndex;
}

template <class T> void Vector<T>::sort()
{
    std::sort(m_data, m_data + m_length);
}

template <class T> void Vector<T>::pushBack(const T &t)
{
    if(m_length >= m_capacity) reserve(m_length * 2 + 4);
    m_data[m_length] = t;
    m_length++;
}

template <class T> void Vector<T>::pushBack(T &&t)
{
    if(m_length >= m_capacity) reserve(m_length * 2 + 4);
    m_data[m_length] = std::move(t);
    m_length++;
}

template <class T> void Vector<T>::pushBack()
{
	if(m_length >= m_capacity) reserve(m_length * 2 + 4);
	m_length++;
}

template <class T> void Vector<T>::popBack()
{
#if defined(MLIB_BOUNDS_CHECK) || defined(DEBUG)
    MLIB_ASSERT_STR(m_length != 0, "PopEnd called on zero-length vector");
#endif
    m_length--;
}

template <class T> void Vector<T>::removeCopy(UINT64 index)
{
#if defined(MLIB_BOUNDS_CHECK) || defined(DEBUG)
    MLIB_ASSERT_STR(index < m_length, "removeCopy called on invalid index");
#endif
    for(UINT64 i = i; i < m_length - 1; i++)
    {
        m_data[i] = m_data[i + 1];
    }
    m_length--;
}

template <class T> void Vector<T>::removeSwap(UINT64 index)
{
#if defined(MLIB_BOUNDS_CHECK) || defined(DEBUG)
    MLIB_ASSERT_STR(index < m_length, "removeSwap called on invalid index");
#endif
    std::swap(m_data[index], m_data[m_length - 1]);
    m_length--;
}

template <class T> void Vector<T>::scale(const T &t)
{
	const UINT64 length = m_length;
	T *ptr = m_data;
	for(UINT64 i = 0; i < length; i++) ptr[i] *= t;
}

template <class T> bool Vector<T>::contains(const T &t) const
{
    const UINT64 length = m_length;
    for(UINT64 i = 0; i < length; i++)
        if(m_data[i] == t)
			return true;
    return false;
}

template <class T> UINT64 Vector<T>::count(const T &t) const
{
    const UINT64 length = m_length;
    UINT64 result = 0;
    for(UINT64 i = 0; i < length; i++)
        if(m_data[i] == t)
            result++;
    return result;
}

template <class T> int Vector<T>::findFirstIndex(const T &t) const
{
    const UINT64 length = m_length;
    for(UINT64 i = 0; i < length; i++)
    {
        if(m_data[i] == t)
        {
            return (int)i;
        }
    }
    return -1;
}

template <class T> void Vector<T>::randomize()
{
    UINT64 length = m_length;
    for(UINT64 i = 0; i < length; i++)
    {
        UINT64 randomNumber = UINT64(rand());
        std::swap(m_data[i], m_data[i + randomNumber % (length - i)]);
    }
}

template <class T> T Vector<T>::dotProduct(const Vector<T> &a, const Vector<T> &b)
{
	MLIB_ASSERT_STR(a.size() == b.size(), "dotProduct called with invalid vectors");
	T result = a[0] * b[0];
	UINT64 length = a.m_length;
	for(UINT64 i = 1; i < length; i++)
	{
		result += a[i] * b[i];
	}
	return result;
}

template <class T> Vector<T> Vector<T>::directProduct(const Vector<T> &a, const Vector<T> &b)
{
	MLIB_ASSERT_STR(a.size() == b.size(), "directProduct called with invalid vectors");
	UINT64 length = a.m_length;
	Vector<T> result(length);
	for(UINT64 i = 0; i < length; i++)
	{
		result[i] = a[i] * b[i];
	}
	return result;
}