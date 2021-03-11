#pragma once

#include "drape/glm_config.hpp"  // Required for <glm> includes below.
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace glsl
{
using glm::dot;
using glm::cross;
using glm::normalize;
using glm::length;
using glm::distance;

using glm::translate;
using glm::rotate;
using glm::scale;
using glm::transpose;
}  // namespace glsl
