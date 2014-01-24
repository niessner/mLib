#pragma once

#ifndef _MLIBBASIC_H_
#define _MLIBBASIC_H_

#define _SCL_SECURE_NO_WARNINGS

#include <exception>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>

#ifndef NOMINMAX
#define NOMINMAX
#endif

//TODO move Console to its own file and replace it with a multistream to a file and the console
class Console
{
public:
	static std::ostream& log()
	{
		return std::cout;
	}

	static void log(const std::string &s)
	{
		log() << s << std::endl;
	}
};

#define MLIB_EXCEPTION(s) std::exception(std::string(__FUNCTION__).append(": ").append(s).c_str())

#ifdef MLIB_ERROR_CHECK

inline void MLIB_WARNING(const char *description)
{
	Console::log() << description << std::endl;
}

inline void MLIB_ERROR(const char *description)
{
	Console::log() << description << std::endl;
}

inline void MLIB_ASSERT(bool statement, const char *description)
{
	if(!statement)
	{
		Console::log() << description << std::endl;
	}
}

inline void MLIB_WARNING(const std::string &description)
{
	Console::log() << description << std::endl;
}

inline void MLIB_ERROR(const std::string &description)
{
	Console::log() << description << std::endl;
}

inline void MLIB_ASSERT(bool statement, const std::string &description)
{
	if(!statement)
	{
		Console::log() << description << std::endl;
	}
}

#else

#define MLIB_WARNING(s)
#define MLIB_ERROR(s)
#define MLIB_ASSERT(b,s)

#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif

#ifndef SAFE_FREE
#define SAFE_FREE(p) { if (p) { free (p);   (p)=NULL; } }
#endif

#ifndef V_RETURN
#define V_RETURN(hr) { if (FAILED(hr)) { return (hr); } }
#endif

#ifndef E_RETURN
#define E_RETURN(hr) { if(FAILED(hr)) { std::cout << #hr << " " << hr << std::endl; while(1); } }
#endif

#ifndef UINT
typedef unsigned int UINT;
#endif

#ifndef UCHAR
typedef unsigned char UCHAR;
#endif

#ifndef INT64
typedef __int64 INT64;
#endif

#ifndef UINT64
typedef unsigned __int64 UINT64;
#endif

#ifndef FLOAT
typedef float FLOAT;
#endif

#ifndef DOUBLE
typedef double DOUBLE;
#endif

#ifndef BYTE
typedef unsigned char BYTE;
#endif

#ifndef USHORT
typedef unsigned short USHORT;
#endif

#endif
