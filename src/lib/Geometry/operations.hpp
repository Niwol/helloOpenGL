#pragma once

#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/IO/MeshIO.hh>

typedef OpenMesh::PolyMesh_ArrayKernelT<> MyMesh;


void laplacianSmoothing(MyMesh& mesh);
