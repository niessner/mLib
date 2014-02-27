
#pragma once
#ifndef _UTILITY_H_
#define _UTILITY_H_

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
	inline bool floatEqual(const T &v0, const T &v1, T eps = (T)0.000001) {
		return (std::abs(v0 - v1) <= eps);
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



	//! clamps between min and max (default between 0 and 1)
	template<class T>
	inline void clamp(T &x, T pMin = T(0.0), T pMax = T(1.0)){
		if (x < pMin)	x = pMin;
		if (x > pMax)	x = pMax;
	}

	template<class T>
	inline T abs(T x)
	{
		if(x < 0) return -x;
		return x;
	}

	template<class T> 
	inline T round( const T &f ) 
	{
		return (f > (T)0.0) ? floor(f + (T)0.5) : ceil(f - (T)0.5);
	}

	template<class T>
	inline bool isPower2(const T &x)
	{
		return (x & (x-1)) == 0;
	}

	template<class T> 
	inline T nextLargeestPow2(T x)
	{
		x |= (x >> 1);
		x |= (x >> 2);
		x |= (x >> 4);
		x |= (x >> 8);
		x |= (x >> 16);
		return(x+1);
	}

	template<class T>
	inline T log2Integer(T x) 
	{
		T r = 0;
		while (x >>= 1)	r++;
		return r;
	}


	//! non-zero 32-bit integer value to compute the log base 10 of 
	template<class T>
	inline T log10Integer(T x) {
		T r;  // result goes here

		const unsigned int PowersOf10[] = 
		{1, 10, 100, 1000, 10000, 100000,
		1000000, 10000000, 100000000, 1000000000};

		T t = (log2Integer(x) + 1) * 1233 >> 12; // (use a lg2 method from above)
		r = t - (x < PowersOf10[t]);
		return r;
	}

	//! returns -1 if negative, 0 if 0, +1 if positive
	template <typename T> 
	inline int sign(T val) {
		return (T(0) < val) - (val < T(0));
	}

	//! returns -1 if negative; +1 otherwise (includes 0)
	template <typename T>
	inline int sgn(T val) {
		return val < 0 ? -1 : 1;
	}

	//! solves a^2 + bx + c = 0
	template<typename T>
	inline void quadraticFormula(T a, T b, T c, T& x0, T& x1) {
		T tmp = (T)-0.5 * (b + (T)sgn(b) * sqrt(b*b - (T)4 * a * c));
		x0 = tmp / a;
		x1 = c / tmp;
	}

}

namespace Utility
{
	//
	// Hashing
	//
    UINT32 hash32(const BYTE *start, UINT length);
    UINT64 hash64(const BYTE *start, UINT length);

    template <class T> inline UINT32 hash32(const T &obj)
    {
        return hash32((const BYTE *)&obj, sizeof(T));
    }

    template <class T> inline UINT64 hash64(const T &obj)
    {
        return hash64((const BYTE *)&obj, sizeof(T));
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
	bool fileExists(const std::string &filename);
	UINT getFileSize(const std::string &filename);
	void copyFile(const std::string &sourceFile, const std::string &destFile);

	//
	// There are OS-specific functions
	//
    void messageBox(const char *string);
    void messageBox(const std::string &S);
    void copyStringToClipboard(const std::string &S);
	std::string loadStringFromClipboard();
	int runCommand(const std::string &executablePath, const std::string &commandLine, bool Blocking);
	void makeDirectory(const std::string &directory);
    
    //
    // Returns the next line in the given file
    //
    std::string getNextLine(std::ifstream &file);
    Vector<BYTE> getFileData(const std::string &filename);

    //
    // Returns the set of all lines in the given file
    //
    Vector<std::string> getFileLines(std::ifstream &file, UINT minLineLength = 0);
    Vector<std::string> getFileLines(const std::string &filename, UINT minLineLength = 0);

    //
	// FILE wrappers
	//
    inline FILE* checkedFOpen(const char *filename, const char *mode)
	{
		FILE *file = fopen(filename, mode);
		MLIB_ASSERT_STR(file != NULL && !ferror(file), std::string("Failed to open file: ") + std::string(filename));
		return file;
	}

    inline void checkedFRead(void *dest, UINT64 elementSize, UINT64 elementCount, FILE *file)
    {
        UINT64 elementsRead = fread(dest, elementSize, elementCount, file);
        MLIB_ASSERT_STR(!ferror(file) && elementsRead == elementCount, "fread failed");
    }

    inline void checkedFWrite(const void *Src, UINT64 elementSize, UINT64 elementCount, FILE *file)
    {
        UINT64 elementsWritten = fwrite(Src, elementSize, elementCount, file);
        MLIB_ASSERT_STR(!ferror(file) && elementsWritten == elementCount, "fwrite failed");
    }

    inline void checkedFSeek(UINT offset, FILE *file)
    {
        int result = fseek(file, offset, SEEK_SET);
        MLIB_ASSERT_STR(!ferror(file) && result == 0, "fseek failed");
    }



	//! uses the <, >  and = operator of the key type
	template<typename Iterator, typename T>
	inline Iterator binarySearch(Iterator& begin, Iterator& end, const T& key) {
		while(begin < end) {
			Iterator middle = begin + (std::distance(begin, end) / 2);

			if (*middle == key)	{	// in that case we exactly found the value
				return middle;
			} else if(*middle > key) {
				end = middle;
			} else {
				begin = middle + 1;
			}
		}

		// if the value is not found return the lower interval
		if (begin < end)	return begin;
		else				return end;
	}
}

#endif // _UTILITY_H_