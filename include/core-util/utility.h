
#pragma once

namespace Math
{
    const double PI = 3.1415926535897932384626433832795028842;
    const float PIf = 3.14159265358979323846f;

    inline float degreesToRadians(float x)
    {
        return x * (PIf / 180.0f);
    }

    inline float radiansToDegrees(float x)
    {
        return x * (180.0f / PIf);
    }

	inline double degreesToRadians(double x)
	{
		return x * (PI / 180.0);
	}

	inline double radiansToDegrees(double x)
	{
		return x * (180.0 / PI);
	}

	template<class T>
	inline T linearMap(T s1, T e1, T s2, T e2, T start)
    {
        return ((start-s1)*(e2-s2)/(e1-s1)+s2);
    }

	template<class T>
    inline T lerp(T Left, T Right, T s)
    {
        return (Left + s * (Right - Left));
    }

    inline int mod(int x, UINT M)
    {
        if(x >= 0) return (x % M);
        else return ((x + (x / int(M) + 2) * int(M)) % M);
    }

	template<class T>
    inline int floor(T x)
    {
        if(x >= 0) return int(x);
        else return int(x) - 1;
    }

	template<class T>
    inline int ceiling(T x)
    {
        int floorX = floor(x);
        if(x == float(floorX)) return floorX;
        else return FloorX + 1;
    }

	template<class T>
    inline int round(T x)
    {
        return int(x + (T)0.5);
    }

    template <class T> inline T square(T x)
    {
        return x * x;
    }

    template <class T> inline T min(T A, T B)
    {
        if(A < B) return A;
        else return B;
    }

    template <class T> inline T min(T A, T B, T C)
    {
        if(A < B && A < C) return A;
        else if(B < C) return B;
        else return C;
    }

    template <class T> inline T max(T A, T B)
    {
        if(A > B) return A;
        else return B;
    }

    template <class T> inline T max(T A, T B, T C)
    {
        if(A > B && A > C) return A;
        else if(B > C) return B;
        else return C;
    }

	template <class T> inline T bound(T value, T low, T high)
	{
		if(value < low) value = low;
		if(value > high) value = high;
		return value;
	}
}

namespace Utility
{
	//
	// Hashing
	//
    UINT32 hash32(const BYTE *start, UINT length);
    UINT64 hash64(const BYTE *start, UINT length);

    template <class T> inline UINT32 hash32(const T &Obj)
    {
        return Hash32((const BYTE *)&Obj, sizeof(T));
    }

    template <class T> inline UINT64 hash64(const T &Obj)
    {
        return Hash64((const BYTE *)&Obj, sizeof(T));
    }

	//
	// Casting
	//
    inline UINT castBoolToUINT(bool b)
    {
        if(b) return 1;
        else return 0;
    }
    
    template <class T> inline BYTE boundToByte(T value)
    {
        if(value < 0) value = 0;
        if(value > 255) value = 255;
        return BYTE(value);
    }

	//
	// file utility
	//
	bool fileExists(const String &filename);
	UINT getFileSize(const String &filename);
	void copyFile(const String &sourceFile, const String &destFile);

	//
	// There are OS-specific functions
	//
    void messageBox(const char *String);
    void messageBox(const String &S);
    void copyStringToClipboard(const String &S);
	String loadStringFromClipboard();
	int runCommand(const String &executablePath, const String &commandLine, bool Blocking);
	void makeDirectory(const String &directory);
    
    //
    // Returns the next line in the given file
    //
    String getNextLine(std::ifstream &file);
    Vector<BYTE> getFileData(const String &filename);

    //
    // Returns the set of all lines in the given file
    //
    Vector<String> getFileLines(std::ifstream &file, UINT minLineLength = 0);
    Vector<String> getFileLines(const String &filename, UINT minLineLength = 0);

    //
	// FILE wrappers
	//
    inline FILE* checkedFOpen(const char *filename, const char *mode)
	{
		FILE *file = fopen(filename, mode);
		MLIB_ASSERT(file != NULL && !ferror(file), String("Failed to open file: ") + String(filename));
		return file;
	}

    inline void checkedFRead(void *dest, UINT64 elementSize, UINT64 elementCount, FILE *file)
    {
        UINT64 elementsRead = fread(dest, elementSize, elementCount, file);
        MLIB_ASSERT(!ferror(file) && elementsRead == elementCount, "fread failed");
    }

    inline void checkedFWrite(const void *Src, UINT64 elementSize, UINT64 elementCount, FILE *file)
    {
        UINT64 elementsWritten = fwrite(Src, elementSize, elementCount, file);
        MLIB_ASSERT(!ferror(file) && elementsWritten == elementCount, "fwrite failed");
    }

    inline void checkedFSeek(UINT offset, FILE *file)
    {
        int result = fseek(file, offset, SEEK_SET);
        MLIB_ASSERT(!ferror(file) && result == 0, "fseek failed");
    }
}