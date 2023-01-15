#include "operations.hpp"

std::vector<std::vector<OpenMesh::SmartVertexHandle>>
computRings(OpenMesh::SmartVertexHandle vertex, uint32_t nbRings)
{
  std::vector<std::vector<OpenMesh::SmartVertexHandle>> rings;
  rings.push_back(std::vector<OpenMesh::SmartVertexHandle>());
  rings[0].push_back(vertex);

  std::vector<OpenMesh::SmartVertexHandle> prevRing = rings[0];

  std::map<int, bool> validationMap;
  validationMap.insert(std::pair<int, bool>(vertex.idx(), true));

  for(uint32_t ring = 1; ring <= nbRings; ring++)
  {

    rings.push_back(std::vector<OpenMesh::SmartVertexHandle>());

    for(auto& v : prevRing)
    {
      for(auto he_iter = v.outgoing_halfedges().begin();
               he_iter != v.outgoing_halfedges().end();
               he_iter++)
      {
        int idx = he_iter->to().idx();
        if(validationMap.find(idx) == validationMap.end())
        {
          rings[ring].push_back(he_iter->to());
          validationMap.insert(std::pair<int, bool>(idx, true));
        }
      }
    }

    prevRing = rings[ring];

    // Removing vertices that are not being checked anymore
    if(ring >= 2)
    {
      for(auto& v : rings[ring - 2])
        validationMap.erase(v.idx());
    }
  }

  return rings;
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

void colorVertices(MyMesh& mesh, MyColor color)
{
  MyMesh::Color uColor = color_floatToUint(color);

  for(auto v_iter = mesh.vertices().begin();
           v_iter != mesh.vertices().end();
           v_iter++)
  {
    mesh.set_color(*v_iter, uColor);
  }
}

void operationOnVertexRegion(MyMesh& mesh, 
                             OpenMesh::SmartVertexHandle vertex,
                             uint32_t nbRings,
                             std::function<void(MyMesh&,
                                                OpenMesh::SmartVertexHandle,
                                                float)> vertexFunc)
{
  auto rings = computRings(vertex, nbRings);

  std::vector<OpenMesh::SmartVertexHandle> allVertices;

  int nbVertices = 0;
  for(auto& ring : rings)
  {
    nbVertices += ring.size();

    for(auto& v : ring)
      allVertices.push_back(v);
  }

  int borderStart = rings[rings.size() - 1].size();
  borderStart = nbVertices - borderStart;

  Eigen::MatrixXf M(nbVertices, nbVertices);
  Eigen::VectorXf B(nbVertices);

  B[0] = 1.0f;
  for(int i = 1; i < nbVertices; i++)
    B[i] = 0.0f;

  for(int i = 0; i < nbVertices; i++)
  {
    for(int j = 0; j < nbVertices; j++)
    {
      if(i == 0)
      {
        M(i, j) = 0.0f;
      }
      else if(i < borderStart)
      {
        bool isNeighbor = false;

        for(auto he_iter = allVertices[i].outgoing_halfedges().begin();
                 he_iter != allVertices[i].outgoing_halfedges().end();
                 he_iter++)
        {
          if(he_iter->to() == allVertices[j])
          {
            isNeighbor = true;
            break;
          }
        }

        if(isNeighbor)
        {
          int valence = allVertices[i].valence();

          M(i, j) = 1.0f / (float)valence;
        }
        else if(i == j)
        {
          M(i, j) = -1.0f;
        }
        else
        {
          M(i, j) = 0.0f;
        }
      }
      else
      {
        if(i == j)
          M(i, j) = 1.0f;
        else
          M(i, j) = 0.0f;
      }
    }
  }
  M(0, 0) = 1.0f;

  Eigen::VectorXf x = M.colPivHouseholderQr().solve(B);

  for(int i = 0; i < borderStart; i++)
    vertexFunc(mesh, allVertices[i], x(i));
}
