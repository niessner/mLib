cmake_minimum_required(VERSION 3.5)

set(TARGET_NAME ml)
project(${TARGET_NAME})

if(UNIX AND NOT APPLE)
    add_definitions(-DLINUX)
endif()

#find dependencies
find_package(ZLIB REQUIRED)

#search folders for source files
set(MLIB_ROOT_HEADERS 
	include/mLibBoost.h
	include/mLibCore.h
	include/mLibDepthCamera.h
	include/mLibEigen.h
	include/mLibFLANN.h
	include/mLibFreeImage.h
	include/mLibLodePNG.h
	include/mLibMBase.h
	include/mLibOpenMesh.h
	include/mLibZLib.h
)
set(MLIB_ROOT_SOURCES 
	include/mLibCore.cpp
	include/mLibDepthCamera.cpp
	include/mLibLodePNG.cpp
	include/mLibZLib.cpp
)

if (WIN32)
	list(APPEND MLIB_ROOT_HEADERS include/mLibD3D11.h include/mLibD3D11Font.h)
	list(APPEND MLIB_ROOT_SOURCES include/mLibD3D11.cpp)
endif()

file(GLOB_RECURSE APPLICATION_BASE_SOURCES src/application-base/*.cpp)
file(GLOB_RECURSE APPLICATION_D3D11_SOURCES src/application-d3d11/*.cpp )

file(GLOB_RECURSE CORE_BASE_SOURCES src/core-base/*.cpp src/core-base/*.tpp)
file(GLOB_RECURSE CORE_GRAPHICS_SOURCES src/core-graphics/*.cpp)
file(GLOB_RECURSE CORE_MATH_SOURCES src/core-math/*.cpp)
file(GLOB_RECURSE CORE_MESH_SOURCES src/core-mesh/*.cpp)
file(GLOB_RECURSE CORE_MULTITHREADING_SOURCES src/core-multithreading/*.cpp)
file(GLOB_RECURSE CORE_UTIL_SOURCES src/core-util/*.cpp)

file(GLOB_RECURSE EXT_LODEPNG_SOURCES src/ext-lodepng/*.cpp)

#search folders for headers
file(GLOB_RECURSE APPLICATION_BASE_HEADERS include/application-base/*.h)
file(GLOB_RECURSE APPLICATION_D3D11_HEADERS include/application-d3d11/*.h include/application-d3d11/*.tpp)

file(GLOB_RECURSE CORE_BASE_HEADERS include/core-base/*.h)
file(GLOB_RECURSE CORE_GRAPHICS_HEADERS include/core-graphics/*.h)
file(GLOB_RECURSE CORE_MATH_HEADERS include/core-math/*.h include/core-math/*.tpp)
file(GLOB_RECURSE CORE_MESH_HEADERS include/core-mesh/*.h)
file(GLOB_RECURSE CORE_MULTITHREADING_HEADERS include/core-multithreading/*.h)
file(GLOB_RECURSE CORE_UTIL_HEADERS include/core-util/*.h)

file(GLOB_RECURSE EXT_ANN_HEADERS include/ext-ANN/*.h)
file(GLOB_RECURSE EXT_BOOST_HEADERS include/ext-boost/*.h)
file(GLOB_RECURSE EXT_CGAL_HEADERS include/ext-cgal/*.h)
file(GLOB_RECURSE EXT_D3D11FONT_HEADERS include/ext-d3d11font/*.h)
file(GLOB_RECURSE EXT_DEPTHCAMERA_HEADERS include/ext-depthcamera/*.h)
file(GLOB_RECURSE EXT_EIGEN_HEADERS include/ext-eigen/*.h)
file(GLOB_RECURSE EXT_FREEIMAGE_HEADERS include/ext-freeimage/*.h)
file(GLOB_RECURSE EXT_LODEPNG_HEADERS include/ext-lodepng/*.h)
file(GLOB_RECURSE EXT_OPENMESH_HEADERS include/ext-openmesh/*.h)
file(GLOB_RECURSE EXT_ZLIB_HEADERS include/ext-zlib/*.h)
file(GLOB_RECURSE EXT_MBASE_HEADERS include/ext-mbase/*.h)


#cross-platform sources
set(${TARGET_NAME}_SOURCE_FILES
		${MLIB_ROOT_SOURCES}
		${MLIB_ROOT_HEADERS}

        ${CORE_BASE_SOURCES}
        ${CORE_BASE_HEADERS}
        ${CORE_GRAPHICS_SOURCES}
        ${CORE_GRAPHICS_HEADERS}
        ${CORE_MATH_SOURCES}
        ${CORE_MATH_HEADERS}
        ${CORE_MESH_SOURCES}
        ${CORE_MESH_HEADERS}
        ${CORE_MULTITHREADING_SOURCES}
        ${CORE_MULTITHREADING_HEADERS}
        ${CORE_UTIL_SOURCES}
        ${CORE_UTIL_HEADERS}

        ${EXT_LODEPNG_SOURCES}

        ${EXT_ANN_HEADERS}
        ${EXT_BOOST_HEADERS}
        ${EXT_CGAL_HEADERS}
        ${EXT_DEPTHCAMERA_HEADERS}
        ${EXT_EIGEN_HEADERS}
        ${EXT_FREEIMAGE_HEADERS}
        ${EXT_LODEPNG_HEADERS}
        ${EXT_OPENMESH_HEADERS}
        ${EXT_ZLIB_HEADERS}
		${EXT_MBASE_HEADERS}
)

#Add DX11 source files on Windows platforms
if(WIN32)
    list(APPEND ${TARGET_NAME}_SOURCE_FILES 
		${APPLICATION_BASE_SOURCES}
		${APPLICATION_D3D11_SOURCES}

		${EXT_D3D11FONT_HEADERS}
		${APPLICATION_BASE_HEADERS}
		${APPLICATION_D3D11_HEADERS}
	)
  
    source_group("ApplicationBase" FILES ${APPLICATION_BASE_SOURCES} ${APPLICATION_BASE_HEADERS})
    source_group("ApplicationD3D11" FILES ${APPLICATION_D3D11_SOURCES} ${APPLICATION_D3D11_HEADERS})
    source_group("ExtD3D11Font" FILES ${EXT_D3D11FONT_HEADERS})
endif()

source_group("MLibRoot" FILES ${MLIB_ROOT_SOURCES} ${MLIB_ROOT_HEADERS})

source_group("CoreBase" FILES ${CORE_BASE_SOURCES} ${CORE_BASE_HEADERS})
source_group("CoreGraphics" FILES ${CORE_GRAPHICS_SOURCES} ${CORE_GRAPHICS_HEADERS})
source_group("CoreMath" FILES ${CORE_MATH_SOURCES} ${CORE_MATH_HEADERS})
source_group("CoreMesh" FILES ${CORE_MESH_SOURCES} ${CORE_MESH_HEADERS})
source_group("CoreMultithreading" FILES ${CORE_MULTITHREADING_SOURCES} ${CORE_MULTITHREADING_HEADERS})
source_group("CoreUtil" FILES ${CORE_UTIL_SOURCES} ${CORE_UTIL_HEADERS})

source_group("ExtANN" FILES ${EXT_ANN_HEADERS})
source_group("ExtBoost" FILES ${EXT_BOOST_HEADERS})
source_group("ExtCGAL" FILES ${EXT_CGAL_HEADERS})

source_group("ExtDepthCamera" FILES ${EXT_DEPTHCAMERA_HEADERS})
source_group("ExtEigen" FILES ${EXT_EIGEN_HEADERS})
source_group("ExtFreeImage" FILES ${EXT_FREEIMAGE_HEADERS})
source_group("ExtLodePNG" FILES ${EXT_LODEPNG_SOURCES} ${EXT_LODEPNG_HEADERS})
source_group("ExtOpenMesh" FILES ${EXT_OPENMESH_HEADERS})
source_group("ExtZLib" FILES ${EXT_ZLIB_HEADERS})


add_library(${TARGET_NAME} ${${TARGET_NAME}_SOURCE_FILES})
target_include_directories(${TARGET_NAME} PUBLIC include ${ZLIB_INCLUDE_DIRS})

target_link_libraries(${TARGET_NAME} ${Boost_LIBRARIES} ${ZLIB_LIBRARIES})