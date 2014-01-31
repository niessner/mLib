
String String::zeroPad(int i, UINT zeroPadding)
{
    return zeroPad(String(i), zeroPadding);
}

String String::zeroPad(const String &S, UINT zeroPadding)
{
	if (zeroPadding == 0) return "";
    String prefix;
    for(UINT i = S.length(); i < zeroPadding; i++) prefix.pushBack('0');
    return prefix + S;
}

bool String::toBool() const
{
	if(*this == "true") return true;
	if(*this == "false") return false;
	MLIB_WARNING("Invalid string to boolean conversion");
	return false;
}

void String::resizeToDataLength()
{
    for(UINT i = 0; i < m_capacity; i++)
    {
        if(m_data[i] == '\0')
        {
            m_length = i;
            resize(m_length + 1);
            return;
        }
    }
    MLIB_ERROR("No null terminator found in resizeToDataLength");
}

String& String::operator = (float value)
{
    std::ostringstream S(std::ostringstream::out);
    S << value;
    *this = S.str().c_str();
    return *this;
}

String& String::operator = (char Character)
{
    allocate(4);
    m_length = 1;
    m_data[0] = Character;
    m_data[1] = '\0';
    return *this;
}

String& String::operator = (int value)
{
    std::ostringstream S(std::ostringstream::out);
    S << value;
    *this = S.str().c_str();
    return *this;
}

String& String::operator = (UINT value)
{
    std::ostringstream S(std::ostringstream::out);
    S << value;
    *this = S.str().c_str();
    return *this;
}

String& String::operator = (UINT64 value)
{
    std::ostringstream S(std::ostringstream::out);
    S << value;
    *this = S.str().c_str();
    return *this;
}

String& String::operator = (const char *S)
{
    UINT newLength = UINT(strlen(S));
    allocate(newLength + 1);
    memcpy(m_data, S, newLength + 1);
    m_length = newLength;
    return *this;
}

String& String::operator = (const String &S)
{
    if(S.m_data == NULL) deleteMemory();
    else if(&S != this)
    {
        allocate(S.m_length + 1);
        m_length = S.m_length;
        memcpy(m_data, S.m_data, m_capacity);
    }
    return *this;
}

String& String::operator += (const String &S)
{
    if(S.m_length > 0)
    {
        const UINT startLength = m_length;
        resize(startLength + S.m_length + 1);
        memcpy(m_data + startLength, S.m_data, S.m_length + 1);
        m_length = startLength + S.m_length;
    }
    return *this;
}

String String::makeLowercase() const
{
    String result(*this);
    for(UINT i = 0; i < result.m_length; i++) result[i] = tolower(result[i]);
    return result;
}

String String::makeUppercase() const
{
    String result(*this);
    for(UINT i = 0; i < result.m_length; i++) result[i] = toupper(result[i]);
    return result;
}

Vector<String> String::split(const String &seperator, bool pushEmptyStrings) const
{
	MLIB_ASSERT(seperator.length() >= 1, "empty seperator");
	Vector<String> result;
	String entry;
	const UINT length = m_length;
	for(UINT i = 0; i < length; i++)
	{
		bool isSeperator = true;
		for(UINT testIndex = 0; testIndex < seperator.length() && i + testIndex < length && isSeperator; testIndex++)
		{
			if(m_data[i + testIndex] != seperator[testIndex])
			{
				isSeperator = false;
			}
		}

		if(isSeperator)
		{
			if(entry.m_length > 0 || pushEmptyStrings)
			{
				result.pushBack(entry);
				entry.m_length = 0;
				entry.m_data[0] = '\0';
			}
			i += seperator.m_length - 1;
		}
		else
		{
			entry.pushBack(m_data[i]);
		}
	}
	if(entry.m_length > 0) result.pushBack(entry);
	return result;
}

Vector<String> String::split(char seperator, bool pushEmptyStrings) const
{
    Vector<String> result;
    String entry;
    const UINT length = m_length;
    for(UINT i = 0; i < length; i++)
    {
        if(m_data[i] == seperator)
        {
            if(entry.m_length > 0 || pushEmptyStrings)
            {
                result.pushBack(entry);
                entry.m_length = 0;
                entry.m_data[0] = '\0';
            }
        }
        else entry.pushBack(m_data[i]);
    }
    if(entry.m_length > 0) result.pushBack(entry);
    return result;
}

int String::findFirstIndex(char seperator) const
{
    const UINT length = m_length;
    for(UINT i = 0; i < length; i++)
        if(m_data[i] == seperator)
            return i;
    return -1;
}

int String::findLastIndex(char seperator) const
{
    const UINT length = m_length;
    for(int i = length - 1; i >= 0; i--)
        if(m_data[i] == seperator)
            return i;
    return -1;
}

bool String::exactMatchAtOffset(const String &find, UINT offset) const
{
	UINT MatchLength = 0;
	for(UINT i = 0; i + offset < m_length && i < find.m_length; i++)
	{
		if(m_data[i + offset] == find[i])
		{
			MatchLength++;
			if(MatchLength == find.m_length) return true;
		}
	}
	return false;
}

bool String::contains(const String &find) const
{
    const UINT length = m_length;
    for(UINT i = 0; i < length; i++)
    {
        if(exactMatchAtOffset(find, i)) return true;
    }
    return false;
}

int String::findFirstIndex(const String &find) const
{
	for(int i = 0; i < (int)m_length; i++)
	{
		if(exactMatchAtOffset(find, i)) return i;
	}
	return -1;
}

String String::findAndReplace(const String &find, const String &replace) const
{
    String result;
    const UINT length = m_length;
    for(UINT i = 0; i < length; i++)
    {
        if(exactMatchAtOffset(find, i))
        {
            result += replace;
            i += find.length() - 1;
        }
        else result += m_data[i];
    }
    return result;
}

bool String::isNumeric() const
{
    return (m_length > 0 &&
           (toDouble() != 0.0 ||
            m_data[0] == '0' ||
            m_data[0] == '.' ||
            m_data[0] == '-'));
}

UINT String::hash32() const
{
	return Utility::hash32((const BYTE *)m_data, m_length);
}

UINT64 String::hash64() const
{
	return Utility::hash64((const BYTE *)m_data, m_length);
}

String String::removeSuffix(const String &endCandidate) const
{
    MLIB_ASSERT(endsWith(endCandidate), "Removing invalid suffix");
    String result = *this;
    for(UINT i = 0; i < endCandidate.m_length; i++) result.popBack();
    return result;
}

String String::removePrefix(const String &startCandidate) const
{
    MLIB_ASSERT(startsWith(startCandidate), "Removing invalid prefix");
    String result = *this;
    for(UINT i = 0; i < startCandidate.m_length; i++) result.PopFront();
    return result;
}

bool String::endsWith(const String &endCanidate) const
{
	if(m_length < endCanidate.m_length) return false;
	for(UINT i = 0; i < endCanidate.m_length; i++)
	{
		if(m_data[m_length - endCanidate.m_length + i] != endCanidate.m_data[i]) return false;
	}
	return true;
}

bool String::startsWith(const String &startCandidate) const
{
    if(m_length < startCandidate.m_length) return false;
    for(UINT i = 0; i < startCandidate.m_length; i++)
    {
        if(m_data[i] != startCandidate.m_data[i]) return false;
    }
    return true;
}

//
// Overloaded operators
//

String operator + (const String &L, const String &R)
{
    String result;
    const UINT totalLength = L.m_length + R.m_length;
    result.allocate(totalLength + 1);
    result.m_length = totalLength;
    result.m_data[totalLength] = '\0';
    if(L.m_length > 0) memcpy(result.m_data, L.m_data, L.m_length);
    if(R.m_length > 0) memcpy(result.m_data + L.m_length, R.m_data, R.m_length);
    return result;
}

std::ostream& operator << (std::ostream &os, const String &S)
{
    if(S.length() > 0) os.write(S.ptr(), S.length());
    return os;
}
