#ifndef MFL_GRAPHICS_UTILITIES_INCLUDED
#define MFL_GRAPHICS_UTILITIES_INCLUDED

#include "graphics_common.hpp"

#include <glm/glm.hpp>

GRAPHICS_BEGIN

glm::vec3 trianglenormal(glm::vec3 const& p1,glm::vec3 const& p2,
                         glm::vec3 const& p3) {
    return glm::normalize(glm::cross(p2-p1,p3-p1));
}

GRAPHICS_END

#endif//MFL_GRAPHICS_UTILITIES_INCLUDED