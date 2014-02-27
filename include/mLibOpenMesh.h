
#pragma once
#ifndef INCLUDE_MLIBOPENMESH_H_
#define INCLUDE_MLIBOPENMESH_H_

// If you link statically against OpenMesh, you have to add
// the define OM_STATIC_BUILD to your application. This will
// ensure that readers and writers get initialized correctly.
#ifndef OM_STATIC_BUILD
#define OM_STATIC_BUILD
#endif

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

//
// ext-openmesh headers
//
#include "ext-openmesh/loader.h"

#endif  // INCLUDE_MLIBOPENMESH_H_
