#pragma once

#ifndef _MLIBCOMMON_H_
#define _MLIBCOMMON_H_

#ifdef WIN32

#define _SCL_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#define _SECURE_SCL 0
#define _HAS_ITERATOR_DEBUGGING 0

#define MLIB_OPENMP

#define DEBUG_BREAK __debugbreak()

#endif

#ifdef LINUX
#define DEBUG_BREAK assert(false)
#endif

#include <exception>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <fstream>
#include <memory>
#include <thread>
#include <mutex>

#ifndef NOMINMAX
#define NOMINMAX
#endif



#if defined (LINUX)
#define __FUNCTION__ __func__
#define __LINE__
#endif

#define FUNCTION_LINE_STRING (String(__FUNCTION__) + ":" + String(__LINE__))

//#define MLIB_EXCEPTION(s) std::exception(((FUNCTION_LINE_STRING) + ": " + String(s)).ptr())
#define MLIB_EXCEPTION(s) std::exception(std::string(__FUNCTION__).append(": ").append(s).c_str())

#ifdef MLIB_ERROR_CHECK


#define MLIB_WARNING(s) warningFunctionMLIB(String(FUNCTION_LINE_STRING) + String() + ": " + String(s))
#define MLIB_ERROR(s) errorFunctionMLIB(String(FUNCTION_LINE_STRING) + ": " + String(s))
#define MLIB_ASSERT(b,s) assertFunctionMLIB(b, String(FUNCTION_LINE_STRING) + ": " + String(s))

void warningFunctionMLIB(const String &description);
void errorFunctionMLIB(const String &description);
void assertFunctionMLIB(bool statement, const String &description);

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

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if (p) { p->Release();   (p)=NULL; } }
#endif

//#ifndef V_RETURN
//#define V_RETURN(hr) { if (FAILED(hr)) { return (hr); } }
//#endif

//#ifndef E_RETURN
//#define E_RETURN(hr) { if(FAILED(hr)) { Console::log() << #hr << " " << hr << std::endl; } }
//#endif

#ifndef D3D_VALIDATE
#define D3D_VALIDATE(statement) { HRESULT hr = statement;  if(FAILED(hr)) { MLIB_ERROR(#statement); } }
#endif

#ifndef UINT
typedef unsigned int UINT;
#endif

#ifndef UCHAR
typedef unsigned char UCHAR;
#endif

#ifndef INT64
#ifdef WIN32
	typedef __int64 INT64;
#else
	typedef int64_t INT64;
#endif
#endif

#ifndef UINT32
#ifdef WIN32
	typedef unsigned __int32 UINT32;
#else
	typedef uint32_t UINT32;
#endif
#endif

#ifndef UINT64
#ifdef WIN32
	typedef unsigned __int64 UINT64;
#else
	typedef uint64_t UINT64;
#endif
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
