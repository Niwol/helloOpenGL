#include "operations.hpp"


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
