#pragma once

#include <memory>
#include <vector>

#include "lib/RenderObject.hpp"
#include "lib/Light/PointLight.hpp"

struct Scene
{
  std::vector<std::shared_ptr<RenderObject>> objects;
  std::vector<PointLight> lights;
};

