
#pragma once
#ifndef _VECTOR_H_
#define _VECTOR_H_

template <class T> class Vector
{
public:

	//
	// Constructors
	//
    Vector()
    {
        m_data = NULL;
        m_length = 0;
        m_capacity = 0;
    }

    explicit Vector(UINT64 length)
    {
        m_data = new T[length];
        m_length = length;
        m_capacity = length;
    }

    Vector(UINT64 size, const T &clearValue)
    {
        m_data = new T[size];
        m_length = size;
        m_capacity = size;
        clear(clearValue);
    }

    Vector(const Vector<T> &V)
    {
        m_length = V.m_length;
        m_capacity = V.m_length;
        if(V.m_length == 0) m_data = NULL;
        else
        {
            m_data = new T[m_length];
            for(UINT64 i = 0; i < m_length; i++) m_data[i] = V.m_data[i];
        }
    }

    Vector(Vector<T> &&V)
    {
        m_length = V.m_length;
        m_capacity = V.m_capacity;
        m_data = V.m_data;
        V.m_length = 0;
        V.m_capacity = 0;
        V.m_data = NULL;
    }

    ~Vector()
    {
        if(m_data != NULL) delete[] m_data;
    }
    
    //
    // Memory
    //
	// also deallocates the memory
    void clear()
    {
        if(m_data != NULL)
        {
            delete[] m_data;
            m_data = NULL;
        }
        m_length = 0;
        m_capacity = 0;
    }

    void zeroMemory()
    {
        if(m_length > 0) memset(m_data, 0, m_length * sizeof(T));
    }

    void operator = (const Vector<T> &V)
    {
        m_length = V.m_length;
        m_capacity = m_length;
        if(m_data != NULL) delete[] m_data;
        if(m_length == 0) m_data = NULL;
        else
        {
            m_data = new T[m_length];
            for(UINT64 i = 0; i < m_length; i++) m_data[i] = V.m_data[i];
        }
    }

    void operator=(Vector<T>&& V)
    {
        if(this != &V)
        {
            if(m_data != NULL) delete[] m_data;
            m_length = V.m_length;
            m_capacity = V.m_capacity;
            m_data = V.m_data;
            V.m_length = 0;
            V.m_capacity = 0;
            V.m_data = NULL;
        }
    }

    void allocate(UINT64 size)
    {
        if(size == 0)
		{
			clear();
			return;
		}
        
        if(m_data != NULL) delete[] m_data;
        m_data = new T[size];
        m_length = size;
        m_capacity = size;
    }

    void allocate(UINT64 size, const T &clearValue)
    {
        allocate(size);
        clear(clearValue);
    }

    void resize(UINT64 size)
    {
        if(size == 0) deleteMemory();
        else if(size <= m_capacity) m_length = size;
        else
        {
            UINT64 newCapacity = size * 3 / 2 + 4;
            T *newData = new T[newCapacity];
            if(m_data != NULL)
            {
                std::move(m_data, m_data + m_length, newData);
                delete[] m_data;
            }
            m_data = newData;
            m_length = size;
            m_capacity = newCapacity;
        }
    }

    void reserve(UINT64 size)
    {
        if(size > m_capacity)
        {
            T *NewData = new T[size];
            if(m_data != NULL)
            {
                std::move(m_data, m_data + m_length, NewData);
                delete[] m_data;
            }
            m_data = NewData;
            m_capacity = size;
        }
    }

    //
    // Accessors
    //
    inline T& operator [] (UINT64 k)
    {
#if defined(MLIB_BOUNDS_CHECK) || defined(_DEBUG)
        if(k >= m_length) throw MLIB_EXCEPTION("Out-of-bounds vector access");
#endif
        return m_data[k];
    }
	inline const T& operator [] (UINT64 k) const
	{
#if defined(MLIB_BOUNDS_CHECK) || defined(_DEBUG)
		if(k >= m_length) throw MLIB_EXCEPTION("Out-of-bounds vector access");
#endif
		return m_data[k];
	}

    inline T& operator [] (int k) 
    {
#if defined(MLIB_BOUNDS_CHECK) || defined(_DEBUG)
        if(k < 0 || k >= int(m_length)) throw MLIB_EXCEPTION("Out-of-bounds vector access");
#endif
        return m_data[k];
    }
    inline const T& operator [] (int k) const
    {
#if defined(MLIB_BOUNDS_CHECK) || defined(_DEBUG)
        if(k < 0 || k >= int(m_length)) throw MLIB_EXCEPTION("Out-of-bounds vector access");
#endif
        return m_data[k];
    }

	inline T& operator [] (UINT k) 
	{
#if defined(MLIB_BOUNDS_CHECK) || defined(_DEBUG)
		if(k >= int(m_length)) throw MLIB_EXCEPTION("Out-of-bounds vector access");
#endif
		return m_data[k];
	}
	inline const T& operator [] (UINT k) const
	{
#if defined(MLIB_BOUNDS_CHECK) || defined(_DEBUG)
		if(k >= int(m_length)) throw MLIB_EXCEPTION("Out-of-bounds vector access");
#endif
		return m_data[k];
	}

    inline UINT64 size() const
    {
        return m_length;
    }
    inline T* ptr()
    {
#if defined(MLIB_BOUNDS_CHECK) || defined(_DEBUG)
        if(m_length == 0) throw MLIB_EXCEPTION("ptr called on zero-length vector");
#endif
        return m_data;
    }
    inline const T* ptr() const
    {
#if defined(MLIB_BOUNDS_CHECK) || defined(_DEBUG)
		if(m_length == 0) throw MLIB_EXCEPTION("ptr called on zero-length vector");
#endif
        return m_data;
    }

	//
	// Random element uses rand() but we should also have a version that uses a RNG object
	//
    inline T& randomElement()
    {
        MLIB_ASSERT(m_length > 0, "RandomElement called with no elements");
        return m_data[rand() % m_length];
    }
    inline const T& randomElement() const
    {
        MLIB_ASSERT(m_length > 0, "RandomElement called with no elements");
        return m_data[rand() % m_length];
    }

    inline T& back()
    {
        MLIB_ASSERT(m_length > 0, "Last called with no elements");
        return m_data[m_length - 1];
    }
    inline const T& back() const
    {
        MLIB_ASSERT(m_length > 0, "Last called with no elements");
        return m_data[m_length - 1];
    }
    inline T& front()
    {
        MLIB_ASSERT(m_length > 0, "First called with no elements");
        return m_data[0];
    }
    inline const T& front() const
    {
        MLIB_ASSERT(m_length > 0, "First called with no elements");
        return m_data[0];
    }
    
    inline T* begin()
    {
        return m_data;
    }
    inline const T* begin() const
    {
        return m_data;
    }
    inline T* end()
    {
        return m_data + m_length;
    }
    inline const T* end() const
    {
        return m_data + m_length;
    }

    //
    // Modifiers
    //
    void pushBack(const T &t);
    void pushBack(T &&t);
	void pushBack();
	void popBack();
    void removeCopy(UINT64 i);
    void removeSwap(UINT64 i);
    void sort();
    template<class orderingType> void sort(orderingType Function)
    {
        if(m_length >= 2)
        {
            std::sort(m_data, m_data + m_length, Function);
        }
    }
	void scale(const T &t);

	//
	// map usage:
	// v.map([](T n) {return String(n);});
	//
    template<class mapFunction>
    auto map(mapFunction function) const -> Vector<decltype(function(std::declval<T>()))>
    {
        Vector<decltype(function(std::declval<T>()))> result(m_length);
        for(UINT64 i = 0; i < m_length; i++) result[i] = function(m_data[i]);
        return result;
    }

	//
	// findFirstIndex usage:
	// v.findFirstIndex([](T n) { return n.startsWith("matt"); });
	// returns the first index matching the predicate, otherwise -1
	//
	template<class predicateFunction>
	int findFirstIndex(predicateFunction function) const
	{
		for(UINT64 i = 0; i < m_length; i++) if(function(m_data[i])) return (int)i;
		return -1;
	}

	//
	// findFirst usage:
	// v.findFirst([](T n) { return n.startsWith("matt"); });
	// returns the first element matching the predicate, otherwise throws an error.
	//
	template<class predicateFunction>
	const T& findFirst(predicateFunction function) const
	{
		for(UINT64 i = 0; i < m_length; i++) if(function(m_data[i])) return m_data[i];
		MLIB_ERROR("entry matching predicate not found");
		return (*m_data);
	}
    
    void clear(const T &clearValue);

	//randomize should also take a RNG
	void randomize();

    //
    // Query
    //
    T sum() const;
    T product() const;

    const T& maxValue() const;
	const T& minValue() const;

    UINT64 maxIndex() const;
	UINT64 minIndex() const;

    UINT64 maxIndex(const std::function<double(const T&)> &valueFunction) const;
    UINT64 minIndex(const std::function<double(const T&)> &valueFunction) const;
    
    bool contains(const T &t) const;
    UINT64 count(const T &t) const;
    int findFirstIndex(const T &t) const;

	static T dotProduct(const Vector<T> &a, const Vector<T> &b);
	static Vector<T> directProduct(const Vector<T> &a, const Vector<T> &b);
    
private:
    T*     m_data;
    UINT64 m_length;
    UINT64 m_capacity;
};

template <class T> inline Vector<T> operator + (const Vector<T> &a, const Vector<T> &b)
{
	MLIB_ASSERT(a.size() == b.size(), "vector add with inconsistent size");
	
	const UINT length = (UINT)a.size();
	Vector<T> result(length);
	for(UINT i = 0; i < length; i++)
		result[i] = a[i] + b[i];
	return result;
}

template <class T> inline Vector<T> operator - (const Vector<T> &a, const Vector<T> &b)
{
	MLIB_ASSERT(a.size() == b.size(), "vector subtract with inconsistent size");

	const UINT64 length = a.size();
	Vector<T> result(length);
	for(UINT64 i = 0; i < length; i++)
		result[i] = a[i] - b[i];
	return result;
}

template <class T, class D> inline Vector<T> operator * (const Vector<T> &a, D b)
{
	const UINT64 length = a.size();
	Vector<T> result(length);
	for(UINT64 i = 0; i < length; i++)
		result[i] = a[i] * b;
	return result;
}

template <class T, class D> inline Vector<T> operator * (D b, const Vector<T> &a)
{
	const UINT length = (UINT)a.size();
	Vector<T> result(length);
	for(UINT i = 0; i < length; i++)
		result[i] = a[i] * b;
	return result;
}

template <class T> inline bool operator == (const Vector<T> &a, const Vector<T> &b)
{
	if(a.size() != b.size()) return false;
	const UINT64 size = a.size();
	for(UINT64 i = 0; i < size; i++)
		if(a[i] != b[i])
			return false;
	return true;
}

template <class T> inline bool operator != (const Vector<T> &a, const Vector<T> &b)
{
	if(a.size() != b.size()) return true;
	const UINT64 size = a.size();
	for(UINT i = 0; i < size; i++)
		if(a[i] != b[i])
			return true;
	return false;
}

#include "../../src/core-base/vector.cpp"


#endif // _VECTOR_H_