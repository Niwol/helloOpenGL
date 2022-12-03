#include "operations.hpp"

std::vector<std::vector<OpenMesh::SmartVertexHandle>>
computRings(MyMesh& mesh, OpenMesh::SmartVertexHandle vertex, uint32_t nbRings)
{
  std::vector<std::vector<OpenMesh::SmartVertexHandle>> rings;

  std::vector<OpenMesh::SmartVertexHandle> prevRing;
  prevRing.push_back(vertex);

  for(uint32_t ring = 0; ring < nbRings; ring++)
  {
    auto first = prevRing[0].outgoing_halfedges().begin()->to();

  }
}

void laplacianSmoothing(MyMesh& mesh)
{
  std::vector<MyMesh::Point> new_pos;

  for(auto v_iter = mesh.vertices().begin();
           v_iter != mesh.vertices().end();
           v_iter++)
  {
    MyMesh::Point p = {0.0f, 0.0f, 0.0f};
    int N = 0;

    for(auto vv_iter = mesh.vv_iter(*v_iter);
             vv_iter.is_valid();
             vv_iter++)
    { 
      p += mesh.point(*vv_iter);
      N++;
    }

    new_pos.push_back(p / (float)N);
  }

  std::vector<MyMesh::Point>::iterator p_iter;
  MyMesh::VertexIter v_iter;

  for(v_iter = mesh.vertices().begin(), p_iter = new_pos.begin();
      v_iter != mesh.vertices().end();
      v_iter++, p_iter++)
  {
    mesh.set_point(*v_iter, *p_iter);
  }
}

void colorVertices(MyMesh& mesh, MyMesh::Color color)
{
  for(auto v_iter = mesh.vertices().begin();
           v_iter != mesh.vertices().end();
           v_iter++)
  {
    mesh.set_color(*v_iter, color);
  }
}

void colorVertexRegion(MyMesh& mesh, OpenMesh::SmartVertexHandle vertex,
                       uint32_t ringLevel,
                       std::function<MyMesh::Color(float)> colorFunc)
{
  auto x = Eigen::VectorXf(ringLevel);
  for(uint32_t i = 0; i < ringLevel; i++)
    x[i] = ringLevel - i;
  x /= ringLevel + 1;

//  std::cout << "Ring level = " << ringLevel << std::endl;
//  std::cout << x << std::endl;

  MyMesh::Color color = colorFunc(1.0f);
  mesh.set_color(vertex, color);

  MyMesh::VertexVertexIter vv_iter = mesh.vv_iter(vertex);
  for(uint32_t ring = 0; ring < ringLevel; ring++)
  {
    color = colorFunc(x[ring]);

    while(vv_iter.is_valid())
    {
      mesh.set_color(*vv_iter, color);
      vv_iter++;
    }
  }
}

void moveVertexRegion(MyMesh& mesh, MyMesh::VertexHandle vertex, 
                      MyMesh::Point offset, uint32_t ringLevel, 
                      std::function<float(float)> colorFunc)
{
  
}
