#pragma once

#ifndef _MLIBCOMMON_H_
#define _MLIBCOMMON_H_

#define _SCL_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <exception>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <fstream>

#ifndef NOMINMAX
#define NOMINMAX
#endif

#define MLIB_EXCEPTION(s) std::exception(std::string(__FUNCTION__).append(": ").append(s).c_str())

#ifdef MLIB_ERROR_CHECK

void MLIB_WARNING(const char *description);
void MLIB_ERROR(const char *description);
void MLIB_ASSERT(bool statement, const char *description);

void MLIB_WARNING(const String &description);
void MLIB_ERROR(const String &description);
void MLIB_ASSERT(bool statement, const String &description);

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
#define E_RETURN(hr) { if(FAILED(hr)) { Console::log() << #hr << " " << hr << std::endl; } }
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

#ifndef UINT32
typedef unsigned __int32 UINT32;
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
