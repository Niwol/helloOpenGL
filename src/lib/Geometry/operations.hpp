#pragma once

#include <functional>

#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/IO/MeshIO.hh>

#include <Eigen/Core>
#include <Eigen/Dense>

typedef OpenMesh::PolyMesh_ArrayKernelT<> MyMesh;
typedef Eigen::Vector3f MyColor;

inline MyMesh::Color color_floatToUint(const MyColor& color)
{
  MyMesh::Color res;
  res[0] = color[0] * 255.0f;
  res[1] = color[1] * 255.0f;
  res[2] = color[2] * 255.0f;

  return res;
}

inline MyColor color_uintToFloat(const MyMesh::Color color)
{
  MyColor res;
  res[0] = (float)color[0] / 255.0f;
  res[1] = (float)color[1] / 255.0f;
  res[2] = (float)color[2] / 255.0f;

  return res;
}

std::vector<std::vector<OpenMesh::SmartVertexHandle>>
computRings(MyMesh& mesh, OpenMesh::SmartVertexHandle vertex, uint32_t nbRings);

void laplacianSmoothing(MyMesh& mesh);

void colorVertices(MyMesh& mesh, MyColor color);

void colorVertexRegion(MyMesh& mesh, OpenMesh::SmartVertexHandle vertex,
                       uint32_t ringLevel,
                       std::function<MyMesh::Color(float)> colorFunc);

void moveVertexRegion(MyMesh& mesh, MyMesh::VertexHandle vertex, 
                      MyMesh::Point offset, uint32_t ringLevel, 
                      std::function<float(float)> colorFunc);

void operationOnVertexRegion(MyMesh& mesh, 
                             OpenMesh::SmartVertexHandle vertex,
                             uint32_t nbRings,
                             std::function<void(MyMesh&,
                                                OpenMesh::SmartVertexHandle,
                                                float)> vertexFunc,
                             std::function<float(float)> weightFunc);

