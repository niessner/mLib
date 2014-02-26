
#pragma once

#include <string.h>

class String
{
public:
    struct LexicographicComparison
    {
        bool operator()(const String &L, const String &R) const
        {
            return strcmp(L.ptr(), R.ptr()) < 0;
        }
        static bool Compare(const String &L, const String &R)
        {
            return strcmp(L.ptr(), R.ptr()) < 0;
        }
    };

    String()
    {
        m_data = NULL;
        m_capacity = 0;
        m_length = 0;
    }

    ~String()
    {
        if(m_data != NULL) delete[] m_data;
    }

    String(const String &S)  {
        if (S.m_data == NULL) {
            m_data = NULL;
            m_capacity = 0;
            m_length = 0;
        } else {
            m_length = S.m_length;
            const UINT newCapacity = m_length + 1;
            m_capacity = newCapacity;
            m_data = new char[newCapacity];
            memcpy(m_data, S.m_data, newCapacity);
        }
    }

    String(String &&S) {
        m_length = S.m_length;
        m_capacity = S.m_capacity;
        m_data = S.m_data;
        S.m_length = 0;
        S.m_capacity = 0;
        S.m_data = NULL;
    }

    explicit String(UINT value)
    {
        m_data = NULL;
        m_capacity = 0;
        m_length = 0;
        *this = value;
    }
    explicit String(UINT64 value)
    {
        m_data = NULL;
        m_capacity = 0;
        m_length = 0;
        *this = value;
    }
    explicit String(int value)
    {
        m_data = NULL;
        m_capacity = 0;
        m_length = 0;
        *this = value;
    }
    explicit String(float value)
    {
        m_data = NULL;
        m_capacity = 0;
        m_length = 0;
        *this = value;
    }
    explicit String(double value)
    {
        m_data = NULL;
        m_capacity = 0;
        m_length = 0;
        *this = float(value);
    }
    String(const char *text)
    {
        m_data = NULL;
        m_capacity = 0;
        m_length = 0;
        *this = text;
    }
    explicit String(char character)
    {
        m_data = NULL;
        m_capacity = 0;
        m_length = 0;
        *this = character;
    }
    explicit String(bool b)
    {
        m_data = NULL;
        m_capacity = 0;
        m_length = 0;
        if(b) *this = "true";
        else *this = "false";
    }

    //
    // Memory
    //
	//! clears the string (also frees the memory)
	inline void clear() {
		deleteMemory();
	}

	//! makes capacity for n characters (newCapacity + '\0')
	inline void reserve(UINT newCapacity) {
		if (newCapacity + 1 > m_capacity) {
			m_capacity = newCapacity + 1;
			char* newData = new char[m_capacity];
			if (m_length > 0) memcpy(newData, m_data, sizeof(char)*(m_length + 1));
			SAFE_DELETE_ARRAY(m_data);
			m_data = newData;
		}
	}

	//! the string is resized to the 'newLength' + '\0'
    inline void resize(UINT newLength) {
        const UINT newCapacity = newLength + 1;
        m_length = std::min(m_length, newLength);
        char *newData = new char[newCapacity];
        
		if (m_length > 0) memcpy(newData, m_data, m_length);
        
		newData[m_length] = '\0';
        
		if(m_data != NULL) delete[] m_data;
        
		m_data = newData;
        m_capacity = newCapacity;
    }

    //
    // Accessors
    //
    inline char* ptr()
    {
        if(m_data != NULL) return m_data;
        return (char *)&(m_data);
    }

    inline const char* ptr() const
    {
		if(m_data != NULL) return m_data;
		return (char *)&(m_data);
    }

	
	// STL defines both size and length for std::string
    //inline UINT size() const
    //{
    //    return m_length;
    //}

	// STL defines both size and length for std::string
	inline UINT length() const
	{
		return m_length;
	}

    inline char last() const
    {
#if defined(MLIB_BOUNDS_CHECK) || defined(DEBUG)
        if(m_length == 0) throw MLIB_EXCEPTION("Last called on zero-length string");
#endif
        return m_data[m_length - 1];
    }

    inline char& operator [] (UINT k)
    {
#if defined(MLIB_BOUNDS_CHECK) || defined(DEBUG)
        if(k >= m_length) throw MLIB_EXCEPTION("Out-of-bounds string access");
#endif
        return m_data[k];
    }

    inline char& operator [] (int k) 
    {
#if defined(MLIB_BOUNDS_CHECK) || defined(DEBUG)
        if(k < 0 || k >= int(m_length)) throw MLIB_EXCEPTION("Out-of-bounds string access");
#endif
        return m_data[k];
    }

    inline const char& operator [] (UINT k) const
    {
#if defined(MLIB_BOUNDS_CHECK) || defined(DEBUG)
        if(k >= m_length) throw MLIB_EXCEPTION("Out-of-bounds string access");
#endif
        return m_data[k];
    }

    inline const char& operator [] (int k) const
    {
#if defined(MLIB_BOUNDS_CHECK) || defined(DEBUG)
        if(k < 0 || k >= int(m_length)) throw MLIB_EXCEPTION("Out-of-bounds string access");
#endif
        return m_data[k];
    }



    //
    // Conversions from string to something else
    //
	char toCHAR() const {
		return ptr()[0];
	}

	unsigned char toUCHAR() const {
		return ptr()[0];
	}

    int toINT() const
    {
        return atoi(ptr());
    }

    UINT toUINT() const
    {
        return UINT(atoi(ptr()));
    }

    INT64 toINT64() const
    {
        return INT64(atoi(ptr()));
    }

    UINT64 toUINT64() const
    {
        return UINT64(atoi(ptr()));
    }

    double toDOUBLE() const
    {
        return atof(ptr());
    }

    float toFLOAT() const
    {
        return float(atof(ptr()));
    }

	bool toBOOL() const 
	{
		if (*this == "false" || *this == "0")	return false;
		else return true;		
	}

    //
    // Query
    //
	bool exactMatchAtOffset(const String &find, UINT offset) const;
    bool contains(const String &find) const;
    bool startsWith(const String &startCanidate) const;
    bool endsWith(const String &endCanidate) const;

    int findFirstIndex(char seperator) const;
    int findLastIndex(char seperator) const;
	int findFirstIndex(const String &find) const;

    String findAndReplace(const String &find, const String &replace) const;

    String makeLowercase() const;
    String makeUppercase() const;

    String removeSuffix(const String &end) const;
    String removePrefix(const String &start) const;

	String removeChar(char c) const;

	bool isNumeric() const;

	UINT hash32() const;
	UINT64 hash64() const;
    
    //
    // Splitting
    //
    Vector<String> split(char seperator, bool pushEmptytrings = false) const;
    Vector<String> split(const String &seperator, bool pushEmptytrings = false) const;

    //
    // Assignment
    //
    String& operator = (const char *value);
    String& operator = (char value);
    String& operator = (float value);
    String& operator = (int value);
    String& operator = (UINT value);
    String& operator = (UINT64 value);
	String& operator = (INT64 value);
    String& operator = (const String &S);

    void operator=(String&& S)
    {
        if(this != &S)
        {
            if(m_data != NULL) delete[] m_data;
            m_length = S.m_length;
            m_capacity = S.m_capacity;
            m_data = S.m_data;
            S.m_length = 0;
            S.m_capacity = 0;
            S.m_data = NULL;
        }
    }

    //
    // Modifiers
    //
    String& operator += (const String &S);
    inline void operator += (char c)
    {
        pushBack(c);
    }

    inline void pushBack(char c)
    {
        if(m_length + 1 >= m_capacity)
        {
            resize(m_capacity * 2 + 3);
        }
        m_data[m_length] = c;
        m_length++;
        m_data[m_length] = '\0';
    }

    inline void popBack()
    {
#if defined(MLIB_BOUNDS_CHECK) || defined(DEBUG)
        if(m_length == 0) throw MLIB_EXCEPTION("Pop called on empty string");
#endif
        m_length--;
        m_data[m_length] = '\0';
    }

    void popFront()
    {
#if defined(MLIB_BOUNDS_CHECK) || defined(DEBUG)
        if(m_length == 0) throw MLIB_EXCEPTION("Pop called on empty string");
#endif
        m_length--;
        for(UINT i = 0; i < m_length; i++) m_data[i] = m_data[i + 1];
        m_data[m_length] = '\0';
    }

    //
    // Formatting
    //
    static String zeroPad(int i, UINT zeroPadding);
    static String zeroPad(const String &S, UINT zeroPadding);
    

	// similar to the stringstream functionality
	template<class T>
	String& operator << (const T& in) {
		*this += String(in);
		return *this;
	}


private:
	inline void deleteMemory()
	{
		if(m_data != NULL)
		{
			delete[] m_data;
			m_data = NULL;
		}
		m_length = 0;
		m_capacity = 0;
	}

	inline void allocate(UINT capacity)
	{
		if(m_data != NULL)
		{
			delete[] m_data;
		}
		m_data = new char[capacity];
		m_data[0] = '\0';
		m_length = 0;
		m_capacity = capacity;
	}


    friend String operator + (const String &L, const String &R);
    friend bool operator == (const String &L, const String &R);
    friend bool operator == (const char *L, const String &R);

	//! call this function to avoid over-allocation (reduces m_capacity to length)
    void resizeToDataLength();
    
    char* m_data;		//data storage (reserved for capacity)
	UINT  m_capacity;	//memory capacity (including space for the '\0')
    UINT  m_length;		//number of characters (not including the '\0')
};

//
// String Comparison
//
inline bool operator == (const String &L, const String &R)
{
    const UINT LLen = L.m_length;
    const UINT RLen = R.m_length;
    if(LLen != RLen) return false;
    
	const char *LData = L.m_data;
    const char *RData = R.m_data;
    for(UINT i = 0; i < LLen; i++)
        if(LData[i] != RData[i])
            return false;
    return true;
}

inline bool operator == (const char *L, const String &R)
{
    const UINT LLen = UINT(strlen(L));
    const UINT RLen = R.m_length;
    if(LLen != RLen) return false;
    
	const char *RData = R.m_data;
    for(UINT i = 0; i < RLen; i++)
        if(L[i] != RData[i])
            return false;
    return true;
}

inline bool operator == (const String &R, const char *L)
{
    return (L == R);
}

inline bool operator != (const String &L, const String &R)
{
    return !(L == R);
}
inline bool operator != (const char *L, const String &R)
{
    return !(L == R);
}
inline bool operator != (const String &R, const char *L)
{
    return !(L == R);
}

inline bool operator < (const String &L, const String &R)
{
    return strcmp(L.ptr(), R.ptr()) < 0;
}

inline bool operator > (const String &L, const String &R)
{
	return strcmp(L.ptr(), R.ptr()) > 0;
}

String operator + (const String &L, const String &R);
std::ostream& operator << (std::ostream &os, const String &S);

