#pragma once

#include <memory>
#include <vector>
#include <set>

#include "lib/RenderObject.hpp"
#include "lib/Light/Light.hpp"

struct Scene
{
  std::set<std::shared_ptr<RenderObject>> objects;
  std::vector<std::shared_ptr<Light>> lights;
};

