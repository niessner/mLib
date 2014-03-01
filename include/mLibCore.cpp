
#ifdef WIN32
#include <windows.h>
#endif

#ifdef LINUX
#define _POSIX_SOURCE
#include <sys/stat.h>
#endif

//
// core-base source files
//
#include "../src/core-base/common.cpp"
#include "../src/core-base/console.cpp"

//
// core-math source files
//
#include "../src/core-math/point.cpp"

//
// core-util source files
//
#include "../src/core-util/utility.cpp"
#include "../src/core-util/directory.cpp"
#include "../src/core-util/clock.cpp"
#include "../src/core-util/pipe.cpp"

//
// core-multithreading source files
//
#include "../src/core-multithreading/threadPool.cpp"
#include "../src/core-multithreading/workerThread.cpp"

//
// core-graphics source files
//
#include "../src/core-graphics/RGBColor.cpp"
#include "../src/core-graphics/camera.cpp"

//
// core-mesh source files
//
#include "../src/core-mesh/meshShapes.cpp"
#include "../src/core-mesh/meshUtil.cpp"