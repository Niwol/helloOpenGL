#pragma once

#include <memory>
#include <vector>

#include "lib/RenderObject.hpp"
#include "lib/Light/Light.hpp"

struct Scene
{
  std::vector<std::shared_ptr<RenderObject>> objects;
  std::vector<std::shared_ptr<Light>> lights;
};

