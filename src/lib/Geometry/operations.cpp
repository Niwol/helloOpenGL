#include "operations.hpp"

bool vecContains(const std::vector<OpenMesh::SmartVertexHandle> &vec, 
                 const OpenMesh::SmartVertexHandle &vertex)
{
  for(auto& v : vec)
  {
    if(v == vertex)
      return true;
  }

  return false;
}

std::vector<std::vector<OpenMesh::SmartVertexHandle>>
computRings(MyMesh& mesh, OpenMesh::SmartVertexHandle vertex, uint32_t nbRings)
{
  std::vector<std::vector<OpenMesh::SmartVertexHandle>> rings;
  rings.push_back(std::vector<OpenMesh::SmartVertexHandle>());
  rings[0].push_back(vertex);

  std::vector<OpenMesh::SmartVertexHandle> prevRing = rings[0];
  std::vector<OpenMesh::SmartVertexHandle> prevPrevRing = rings[0];

  for(uint32_t ring = 1; ring <= nbRings; ring++)
  {
    // Find first half edge
    OpenMesh::SmartHalfedgeHandle currentHalfEdge;
    if(ring == 1)
    {
      currentHalfEdge = mesh.halfedge_handle(vertex);
    }
    else
    {
      // for(auto halfEdge_iter = mesh.voh_begin(prevRing[0]);
      //          halfEdge_iter != mesh.voh_end(prevRing[0]);
      //          halfEdge_iter++)
      // {
      //   if(halfEdge_iter->to() == prevRing[1])
      //   {
      //     currentHalfEdge = *halfEdge_iter;
      //     break;
      //   }
      // }

      // currentHalfEdge = currentHalfEdge.opp();

      // while(vecContains(prevRing, currentHalfEdge.to()))
      //   currentHalfEdge = currentHalfEdge.next();

      bool vertexFound = false;
      int serachIdx = 0;

      while(!vertexFound)
      {
        for(auto halfEdge_iter = mesh.voh_begin(prevRing[serachIdx]);
                 halfEdge_iter != mesh.voh_end(prevRing[serachIdx]);
                 halfEdge_iter++)
        {
          if((vecContains(prevPrevRing, halfEdge_iter->to()) ||
              vecContains(prevRing, halfEdge_iter->to())) == false)
          {
            currentHalfEdge = *halfEdge_iter;
            vertexFound = true;
            break;
          }
        }

        serachIdx++;
      }
    }

    // Init vertex start
    auto firstVertex = currentHalfEdge.to();
    auto currentVertex = currentHalfEdge.to();

    // Prepare current ring
    rings.push_back(std::vector<OpenMesh::SmartVertexHandle>());

    do
    {
      if(vecContains(prevRing, currentVertex))
      {
        currentHalfEdge = currentHalfEdge.opp();
      }
      else 
      {
        rings[ring].push_back(currentVertex);
      }

      currentHalfEdge = currentHalfEdge.next();
      currentVertex = currentHalfEdge.to();
    }
    while(currentVertex != firstVertex);

    prevPrevRing = prevRing;
    prevRing = rings[ring];
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

  auto createColor = [](float r, float g, float b)
  {
      MyColor fColor = {r, g, b};
      MyMesh::Color color = color_floatToUint(fColor);

      return color;
  };

  std::vector<MyMesh::Color> colors;
  colors.push_back(createColor(0.0, 0.0, 0.0));
  colors.push_back(createColor(1.0, 0.0, 0.0));
  colors.push_back(createColor(0.0, 1.0, 0.0));
  colors.push_back(createColor(0.0, 0.0, 1.0));
  colors.push_back(createColor(1.0, 0.0, 1.0));
  colors.push_back(createColor(1.0, 1.0, 0.0));
  colors.push_back(createColor(0.0, 1.0, 1.0));

  auto rings = computRings(mesh, vertex, ringLevel);

  int i = 0;
  for(auto& ring : rings)
  {
    float x = (float)i / rings.size();
    auto color = colorFunc(x);

    for(auto& v : ring)
    {
      mesh.set_color(v, color);
    }
    i++;
  }
  
  colorVertices(mesh, {0.0f, 0.0f, 0.0f});

  i = 0;
  for(auto& ring : rings)
  {
    float x = (float)i / rings.size();
    auto color = colorFunc(x);

    for(auto& v : ring)
    {
      mesh.set_color(v, colors[i]);
    }
    i++;
  }
}

void moveVertexRegion(MyMesh& mesh, MyMesh::VertexHandle vertex, 
                      MyMesh::Point offset, uint32_t ringLevel, 
                      std::function<float(float)> colorFunc)
{
  
}

void operationOnVertexRegion(MyMesh& mesh, 
                             OpenMesh::SmartVertexHandle vertex,
                             uint32_t nbRings,
                             std::function<void(MyMesh&,
                                                OpenMesh::SmartVertexHandle,
                                                float)> vertexFunc,
                             std::function<float(float)> weightFunc)
{
  auto rings = computRings(mesh, vertex, nbRings);

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
