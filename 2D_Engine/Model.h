#pragma once

#include <assimp/Importer.hpp>
#include <string>
#include <vector>
#include "Animation.h"
#include "Skeleton.h"
#include "SkinVertex.h"

// A model.
class Model 
{
    public:
        // Constructor. Empty model.
        Model();

        // Constructor.
        // path Path to file.
        Model(std::string path);

        // Destructor.
        ~Model();

        // Load model.
        // path Path to file.
        void Load(std::string path);

        // Vertex list (bind pose).
        std::vector<Geometry::SkinVertex> mVerticesBindPose;

        // Vertex list.
        std::vector<Geometry::SkinVertex> mVertices;

        // Vertex list.
        std::vector<unsigned int> mIndices;

        // Assimp importer.
        static Assimp::Importer aImporter;

        // Skeleton to animate.
        Geometry::Skeleton skeleton;

        // Animations to animate skeleton.
        std::vector<Geometry::Animation> animations;

        // Transform mesh on CPU.
        void TransformMeshCPU();

    private:
        struct MeshEntry {
            unsigned int numIndices = 0;
            unsigned int baseVertex = 0;
            unsigned int baseIndex = 0;
        };

        void LoadMeshes(const aiScene* aScene);
        void LoadAnimations(const aiScene* aScene);

        // Skin vertices on CPU. Used only for debugging purposes.
        void MeshTransform(const std::vector<glm::mat4>& transforms, const std::vector<glm::mat3>& transformsIT);

};
