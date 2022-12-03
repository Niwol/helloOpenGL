#pragma once

#include <functional>

#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/IO/MeshIO.hh>

#include <Eigen/Core>

typedef OpenMesh::PolyMesh_ArrayKernelT<> MyMesh;

std::vector<std::vector<OpenMesh::SmartVertexHandle>>
computRings(MyMesh& mesh, OpenMesh::SmartVertexHandle vertex, uint32_t nbRings);

void laplacianSmoothing(MyMesh& mesh);

void colorVertices(MyMesh& mesh, MyMesh::Color color);

void colorVertexRegion(MyMesh& mesh, OpenMesh::SmartVertexHandle vertex,
                       uint32_t ringLevel,
                       std::function<MyMesh::Color(float)> colorFunc);

void moveVertexRegion(MyMesh& mesh, MyMesh::VertexHandle vertex, 
                      MyMesh::Point offset, uint32_t ringLevel, 
                      std::function<float(float)> colorFunc);

