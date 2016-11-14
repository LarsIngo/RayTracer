#pragma once

#include <glm/glm.hpp>

namespace Geometry {
    struct SkinVertex {
        /// Position.
        glm::vec3 position;
        /// %Texture coordinate.
        glm::vec2 textureCoordinate;
        /// Normal.
        glm::vec3 normal;
        /// Tangent vector.
        glm::vec3 tangent;
        /// Bone IDs.
        glm::ivec4 boneIDs;
        /// Bone weights.
        glm::vec4 weights;
    };
}
