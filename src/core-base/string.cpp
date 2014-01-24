/*
String.cpp
Written by Matthew Fisher

Implementation of String class.
*/

String::String(const UnicodeString &S)
{
    const UINT SLength = S.Length();
    _Length = SLength;
    _Capacity = _Length + 1;
    _Data = new char[_Length + 1];
    for(UINT Index = 0; Index < SLength; Index++)
    {
        if(S[Index] >= 128)
        {
            _Data[Index] = '#';
        }
        else
        {
            _Data[Index] = char(S[Index]);
        }
    }
    _Data[SLength] = '\0';
}

String String::UnsignedIntAsHex(UINT Value)
{
    String S;
    S.Allocate(20);
    sprintf(S.CString(), "%x", Value);
    S.ResizeToCStringLength();
    return S;
}

String String::Hash32ToHexString(UINT32 hash)
{
    String result("xxxxxxxx");
    BYTE *stream = (BYTE *)&hash;
    ByteStreamToHexString(stream, 4, result);
    return result;
}

String String::Hash64ToHexString(UINT64 hash)
{
    String result("xxxxxxxxxxxxxxxx");
    BYTE *stream = (BYTE *)&hash;
    ByteStreamToHexString(stream, 8, result);
    return result;
}

UINT32 String::HexStringToHash32(const String &s)
{
    Assert(s._Length == 8, "Invalid hash length");
    UINT32 result;
    HexStringToByteStream(s, (BYTE *)&result);
    return result;
}

UINT64 String::HexStringToHash64(const String &s)
{
    Assert(s._Length == 16, "Invalid hash length");
    UINT64 result;
    HexStringToByteStream(s, (BYTE *)&result);
    return result;
}

void String::ByteStreamToHexString(const BYTE *stream, UINT count, String &result)
{
    static const char table[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    if(result._Length != count * 2)
    {
        result.Allocate(count * 2 + 1);
        result._Length = count * 2;
    }
    char *resultData = result._Data;
    UINT stringIndex = 0;
    for(UINT byteIndex = 0; byteIndex < count; byteIndex++)
    {
        const BYTE curByte = stream[byteIndex];
        resultData[stringIndex++] = table[ curByte       & 0x0F];
        resultData[stringIndex++] = table[(curByte >> 4) & 0x0F];
    }
    resultData[stringIndex++] = '\0';
}

String String::ByteStreamToHexString(const BYTE *stream, UINT count)
{
    static const char table[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    String result;
    result.Allocate(count * 2 + 1);
    result._Length = count * 2;
    char *resultData = result._Data;
    UINT stringIndex = 0;
    for(UINT byteIndex = 0; byteIndex < count; byteIndex++)
    {
        const BYTE curByte = stream[byteIndex];
        resultData[stringIndex++] = table[ curByte       & 0x0F];
        resultData[stringIndex++] = table[(curByte >> 4) & 0x0F];
    }
    resultData[stringIndex++] = '\0';
    return result;
}

__forceinline UINT HexStringToByteStreamHelper(char c)
{
    if(c >= '0' && c <= '9')
    {
        return UINT(c) - '0';
    }
    if(c >= 'a' && c <= 'f')
    {
        return 10 + UINT(c) - 'a';
    }
    return 0;
}

void String::HexStringToByteStream(const String &hexString, BYTE *result)
{
    const UINT length = hexString._Length / 2;
    const char *hexStringData = hexString._Data;
    UINT stringIndex = 0;
    for(UINT byteIndex = 0; byteIndex < length; byteIndex++)
    {
        result[byteIndex] =  HexStringToByteStreamHelper(hexStringData[stringIndex++]);
        result[byteIndex] |= HexStringToByteStreamHelper(hexStringData[stringIndex++]) << 4;
    }
}

String String::ZeroPad(int i, UINT ZeroPadding)
{
    return ZeroPad(String(i), ZeroPadding);
}

String String::ZeroPad(const String &S, UINT ZeroPadding)
{
	if (ZeroPadding == 0) return "";
    String Prefix;
    for(UINT i = S.Length(); i < ZeroPadding; i++)
    {
        Prefix.PushEnd('0');
    }
    return Prefix + S;
}

bool String::ConvertToBoolean() const
{
    if(*this == "true")
    {
        return true;
    }
    if(*this == "false")
    {
        return false;
    }
    SignalError("Invalid string to boolean conversion");
    return false;
}

void String::ResizeToCStringLength()
{
    for(UINT i = 0; i < _Capacity; i++)
    {
        if(_Data[i] == '\0')
        {
            _Length = i;
            ReSize(_Length + 1);
            return;
        }
    }
    SignalError("No null terminator found in String::ResizeToCStringLength");
}

String& String::operator = (float Value)
{
    ostringstream S(ostringstream::out);
    S << Value;
    *this = S.str().c_str();
    return *this;
}

String& String::operator = (char Character)
{
    Allocate(4);
    _Length = 1;
    _Data[0] = Character;
    _Data[1] = '\0';
    return *this;
}

String& String::operator = (int Value)
{
    ostringstream S(ostringstream::out);
    S << Value;
    *this = S.str().c_str();
    return *this;
}

String& String::operator = (UINT Value)
{
    ostringstream S(ostringstream::out);
    S << Value;
    *this = S.str().c_str();
    return *this;
}

String& String::operator = (UINT64 Value)
{
    ostringstream S(ostringstream::out);
    S << Value;
    *this = S.str().c_str();
    return *this;
}

String& String::operator = (const char *String)
{
    UINT NewLength = UINT(strlen(String));
    Allocate(NewLength + 1);
    memcpy(_Data, String, NewLength + 1);
    _Length = NewLength;
    return *this;
}

String& String::operator = (const String &S)
{
    if(S._Data == NULL)
    {
        FreeMemory();
    }
    else if(&S != this)
    {
        Allocate(S._Length + 1);
        _Length = S._Length;
        memcpy(_Data, S._Data, _Capacity);
    }
    return *this;
}

String& String::operator += (const String &S)
{
    if(S._Length > 0)
    {
        const UINT StartLength = _Length;
        ReSize(StartLength + S._Length + 1);
        memcpy(_Data + StartLength, S._Data, S._Length + 1);
        _Length = StartLength + S._Length;
    }
    return *this;
}

String String::MakeLowercase() const
{
    String Result(*this);
    for(UINT i = 0; i < Result._Length; i++)
    {
        Result[i] = tolower(Result[i]);
    }
    return Result;
}

String String::MakeUppercase() const
{
    String Result(*this);
    for(UINT i = 0; i < Result._Length; i++)
    {
        Result[i] = toupper(Result[i]);
    }
    return Result;
}

void String::Partition(char Seperator, Vector<String> &Output, bool PushEmptyStrings) const
{
    Output.FreeMemory();
    String CurEntry;
    const UINT Length = _Length;
    for(UINT Index = 0; Index < Length; Index++)
    {
        if(_Data[Index] == Seperator)
        {
            if(CurEntry._Length > 0 || PushEmptyStrings)
            {
                Output.PushEnd(CurEntry);
                CurEntry._Length = 0;
                CurEntry._Data[0] = '\0';
            }
        }
        else
        {
            CurEntry.PushEnd(_Data[Index]);
        }
    }
    if(CurEntry._Length > 0)
    {
        Output.PushEnd(CurEntry);
    }
}

Vector<String> String::Partition(char Seperator) const
{
    Vector<String> Result;
    Partition(Seperator, Result);
    return Result;
}

int String::FindFirstIndex(char Seperator) const
{
    UINT L = Length();
    for(UINT CurIndex = 0; CurIndex < L; CurIndex++)
    {
        if(_Data[CurIndex] == Seperator)
        {
            return CurIndex;
        }
    }
    return -1;
}

int String::FindLastIndex(char Seperator) const
{
    UINT L = Length();
    for(int CurIndex = L - 1; CurIndex >= 0; CurIndex--)
    {
        if(_Data[CurIndex] == Seperator)
        {
            return CurIndex;
        }
    }
    return -1;
}

Vector<String> String::PartitionAboutIndex(UINT index) const
{
    Vector<String> result(2);
    PartitionAboutIndex(index, result[0], result[1]);
    return result;
}

void String::PartitionAboutIndex(UINT Index, String &Left, String &Right) const
{
    UINT L = Length();
    Assert(Index < L, "Index invalid in String::PartitionAboutIndex");
    
    Left.ReSize(Index);
    for(UINT LeftIndex = 0; LeftIndex < Index; LeftIndex++)
    {
        Left._Data[LeftIndex] = _Data[LeftIndex];
    }
    Left._Data[Index] = '\0';
    Left._Length = Index;

    const UINT RLength = L - Index - 1;
    Right.ReSize(RLength);
    for(UINT RightIndex = 0; RightIndex < RLength; RightIndex++)
    {
        Right._Data[RightIndex] = _Data[Index + 1 + RightIndex];
    }
    Right._Data[RLength] = '\0';
    Right._Length = RLength;
}

Vector<String> String::Partition(const String &seperator) const
{
    Vector<String> result;
    Partition(seperator, result);
    return result;
}

void String::Partition(const String &Seperator, Vector<String> &Output) const
{
    Assert(Seperator.Length() >= 1, "empty seperator");
    Output.FreeMemory();
    String CurEntry;
    for(UINT CurIndex = 0; CurIndex < Length(); CurIndex++)
    {
        bool IsSeperator = true;
        for(UINT TestIndex = 0; TestIndex < Seperator.Length() && CurIndex + TestIndex < Length() && IsSeperator; TestIndex++)
        {
            if(_Data[CurIndex + TestIndex] != Seperator[TestIndex])
            {
                IsSeperator = false;
            }
        }

        if(IsSeperator)
        {
            if(CurEntry._Length > 0)
            {
                Output.PushEnd(CurEntry);
                CurEntry._Length = 0;
                CurEntry._Data[0] = '\0';
            }
            CurIndex += Seperator._Length - 1;
        }
        else
        {
            CurEntry.PushEnd(_Data[CurIndex]);
        }
    }
    if(CurEntry._Length > 0)
    {
        Output.PushEnd(CurEntry);
    }
}

bool String::ExactMatchAtOffset(const String &Find, UINT Offset) const
{
    UINT MatchLength = 0;
    for(UINT Index = 0; Index + Offset < _Length && Index < Find._Length; Index++)
    {
        if(_Data[Index + Offset] == Find[Index])
        {
            MatchLength++;
            if(MatchLength == Find._Length)
            {
                return true;
            }
        }
    }
    return false;
}

bool String::Contains(const String &Find) const
{
    const UINT Length = _Length;
    for(UINT Index = 0; Index < Length; Index++)
    {
        if(ExactMatchAtOffset(Find, Index))
        {
            return true;
        }
    }
    return false;
}

int String::FindFirstIndex(const String &Find) const
{
	for(int Index = 0; Index < (int)_Length; Index++)
	{
		if(ExactMatchAtOffset(Find, Index))
		{
			return Index;
		}
	}
	return -1;
}

String String::FindAndReplace(const String &Find, const String &Replace) const
{
    String Result;
    const UINT Length = _Length;
    for(UINT Index = 0; Index < Length; Index++)
    {
        if(ExactMatchAtOffset(Find, Index))
        {
            Result += Replace;
            Index += Find.Length() - 1;
        }
        else
        {
            Result += _Data[Index];
        }
    }
    return Result;
}

bool String::IsNumeric() const
{
    return (_Length > 0 &&
           (ConvertToDouble() != 0.0 ||
            _Data[0] == '0' ||
            _Data[0] == '.' ||
            _Data[0] == '-'));
}

bool String::EndsWith(const String &EndCanidate) const
{
    if(_Length < EndCanidate._Length)
    {
        return false;
    }
    for(UINT i = 0; i < EndCanidate._Length; i++)
    {
        if(_Data[_Length - EndCanidate._Length + i] != EndCanidate._Data[i])
        {
            return false;
        }
    }
    return true;
}

String String::RemoveSuffix(const String &EndCandidate) const
{
    Assert(EndsWith(EndCandidate), "Removing invalid suffix");
    String Result = *this;
    for(UINT i = 0; i < EndCandidate._Length; i++)
    {
        Result.PopEnd();
    }
    return Result;
}

String String::RemovePrefix(const String &StartCandidate) const
{
    Assert(StartsWith(StartCandidate), "Removing invalid prefix");
    String Result = *this;
    for(UINT i = 0; i < StartCandidate._Length; i++)
    {
        Result.PopFront();
    }
    return Result;
}

bool String::StartsWith(const char *StartCandidate) const
{
    for(UINT i = 0; i <= _Length; i++)
    {
        char c = StartCandidate[i];
        if(c == 0)
        {
            return true;
        }
        else if(c != _Data[i])
        {
            return false;
        }
    }
    return false;
}

bool String::StartsWith(const String &StartCanidate) const
{
    if(_Length < StartCanidate._Length)
    {
        return false;
    }
    for(UINT i = 0; i < StartCanidate._Length; i++)
    {
        if(_Data[i] != StartCanidate._Data[i])
        {
            return false;
        }
    }
    return true;
}

UINT32 String::Hash32() const
{
    return Utility::Hash32((const BYTE *)_Data, _Length);
}

//
// Overloaded operators
//

String operator + (const String &L, const String &R)
{
    String Result;
    const UINT TotalLength = L._Length + R._Length;
    Result.Allocate(TotalLength + 1);
    Result._Length = TotalLength;
    Result._Data[TotalLength] = '\0';
    if(L._Length > 0)
    {
        memcpy(Result._Data, L._Data, L._Length);
    }
    if(R._Length > 0)
    {
        memcpy(Result._Data + L._Length, R._Data, R._Length);
    }
    return Result;
}

/*String operator + (const char *L, const String &R)
{
    String Ret = L;
    for(UINT i = 0; i < R.Length(); i++)
    {
        Ret.PushEnd(R[i]);
    }
    return Ret;
}

String operator + (const String &L, const char *R)
{
    UINT RLen = UINT(strlen(R));
    String Ret = L;
    for(UINT i = 0; i < RLen; i++)
    {
        Ret.PushEnd(R[i]);
    }
    return Ret;
}*/

ostream& operator << (ostream &os, const String &S)
{
    if(S.Length() > 0)
    {
        os.write(S.CString(), S.Length());
    }
    return os;
}
