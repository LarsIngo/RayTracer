#include "Model.h"

#include "MathFunctions.h"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

Assimp::Importer Model::aImporter = Assimp::Importer();

using namespace Geometry;

Model::Model() 
{

}

Model::Model(std::string path) 
{
    Load(path);
}

Model::~Model() 
{

}

void Model::Load(std::string path) 
{
    mVertices.clear();

    // Load geometry data.
    const aiScene* aScene = aImporter.ReadFile(path,
        aiProcess_CalcTangentSpace |
        aiProcess_GenSmoothNormals |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ImproveCacheLocality |
        aiProcess_LimitBoneWeights |
        aiProcess_RemoveRedundantMaterials |
        aiProcess_SplitLargeMeshes |
        aiProcess_Triangulate |
        aiProcess_GenUVCoords |
        aiProcess_SortByPType |
        aiProcess_FindDegenerates |
        aiProcess_FindInvalidData |
        aiProcess_ValidateDataStructure |
        0);

    std::cout << aImporter.GetErrorString() << std::endl;

    assert(aScene != nullptr);

    // Load skeleton.
    skeleton.Load(aScene);

    // Load meshes.
    LoadMeshes(aScene);

    // Load animation data.
    LoadAnimations(aScene);

    // Set model to bind pose.
    skeleton.BindPose();

    // Release scene.
    aImporter.FreeScene();

    //mVertices.resize(6);

    //mVertices[0] = Vertex(glm::vec3(-5.f, -5.f, 5.f), glm::vec3(0.f, 0.f, -1.f), glm::vec2(0.f, 1.f));
    //mVertices[1] = Vertex(glm::vec3(-5.f, 5.f, 5.f), glm::vec3(0.f, 0.f, -1.f), glm::vec2(0.f, 0.f));
    //mVertices[2] = Vertex(glm::vec3(5.f, 5.f, 5.f), glm::vec3(0.f, 0.f, -1.f), glm::vec2(1.f, 0.f));

    //mVertices[3] = Vertex(glm::vec3(5.f, 5.f, 5.f), glm::vec3(0.f, 0.f, -1.f), glm::vec2(1.f, 0.f));
    //mVertices[4] = Vertex(glm::vec3(5.f, -5.f, 5.f), glm::vec3(0.f, 0.f, -1.f), glm::vec2(1.f, 1.f));
    //mVertices[5] = Vertex(glm::vec3(-5.f, -5.f, 5.f), glm::vec3(0.f, 0.f, -1.f), glm::vec2(0.f, 1.f));
}

void Model::LoadMeshes(const aiScene* aScene) 
{
    std::vector<MeshEntry> entries;
    entries.resize(aScene->mNumMeshes);

    std::size_t numVertices = 0;
    std::size_t numIndices = 0;

    // Count the number of vertices and indices.
    for (unsigned int i = 0; i < aScene->mNumMeshes; ++i) {
        entries[i].numIndices = aScene->mMeshes[i]->mNumFaces * 3;
        entries[i].baseVertex = numVertices;
        entries[i].baseIndex = numIndices;

        numVertices += aScene->mMeshes[i]->mNumVertices;
        numIndices += entries[i].numIndices;
    }

    // Resize vectors to fit.
    mVertices.resize(numVertices);
    mVerticesBindPose.resize(numVertices);
    mIndices.resize(numIndices);

    std::vector<unsigned int> weightCounter(numVertices, 0);
    numVertices = 0;
    numIndices = 0;
    // Initialize the meshes in the scene one by one.
    for (unsigned int m = 0; m < aScene->mNumMeshes; ++m) {
        const aiMesh* aMesh = aScene->mMeshes[m];
        // Load vertices.
        for (unsigned int i = 0; i < aMesh->mNumVertices; ++i) {
            SkinVertex& vert = mVertices[numVertices];
            CpyVec(vert.position, aMesh->mVertices[i]);
            if (aMesh->HasTextureCoords(0))
                CpyVec(vert.textureCoordinate, aMesh->mTextureCoords[0][i]);
            else
                vert.textureCoordinate = glm::vec2(0.f, 0.f);
            CpyVec(vert.normal, aMesh->mNormals[i]);
            if (aMesh->HasTangentsAndBitangents())
                CpyVec(vert.tangent, aMesh->mTangents[i]);
            else
                vert.tangent = glm::vec3(0.f, 0.f, 0.f);
            mVerticesBindPose[numVertices] = vert;
            ++numVertices;
        }
        // Load indices.
        for (unsigned int i = 0; i < aMesh->mNumFaces; ++i) {
            const aiFace& aFace = aMesh->mFaces[i];
            assert(aFace.mNumIndices == 3);
            mIndices[numIndices++] = entries[m].baseVertex + aFace.mIndices[0];
            mIndices[numIndices++] = entries[m].baseVertex + aFace.mIndices[1];
            mIndices[numIndices++] = entries[m].baseVertex + aFace.mIndices[2];
        }
        // Load Weights.
        assert(skeleton.GetNumBones() != 0 || aMesh->mNumBones == 0); // Check if skeleton is loaded.
        for (unsigned int b = 0; b < aMesh->mNumBones; ++b) {
            const aiBone* aBone = aMesh->mBones[b];
            const size_t boneIndex = skeleton.FindBoneIndex(aBone->mName.data);
            assert(boneIndex != -1);
            for (unsigned int i = 0; i < aBone->mNumWeights; ++i) {
                unsigned int vertexID = entries[m].baseVertex + aBone->mWeights[i].mVertexId;
                float weight = aBone->mWeights[i].mWeight;
                unsigned int& count = weightCounter[vertexID];
                assert(count + 1 <= 4);
                mVertices[vertexID].boneIDs[count] = boneIndex;
                mVertices[vertexID].weights[count] = weight;
                mVerticesBindPose[vertexID].boneIDs[count] = boneIndex;
                mVerticesBindPose[vertexID].weights[count] = weight;
                ++count;
            }
        }
    }
}

void Model::LoadAnimations(const aiScene* aScene) 
{
    animations.resize(aScene->mNumAnimations);

    for (std::size_t a = 0; a < animations.size(); ++a) 
    {
        animations[a].Load(aScene->mAnimations[a]);
    }
}

// Transform mesh on CPU.
void Model::TransformMeshCPU()
{
    const std::vector<glm::mat4>& transforms = skeleton.GetFinalTransformations();
    const std::vector<glm::mat3>& transformsIT = skeleton.GetFinalTransformationsIT();
    MeshTransform(transforms, transformsIT);
}

void Model::MeshTransform(const std::vector<glm::mat4>& bones, const std::vector<glm::mat3>& bonesIT)
{
    // CPU-skinning.
    unsigned int vertexBoneIDs[4];
    float vertexWeights[4];

    // Vertex shader
    for (std::size_t v = 0; v < mVertices.size(); ++v) {
        for (unsigned int i = 0; i < 4; ++i) {
            vertexBoneIDs[i] = mVertices[v].boneIDs[i];
            vertexWeights[i] = mVertices[v].weights[i];
        }

        glm::vec3 vertexPosition = mVerticesBindPose[v].position;
        glm::vec3 vertexNormal = mVerticesBindPose[v].normal;

        //assert(abs(1.f - vertexWeights[0] + vertexWeights[1] + vertexWeights[2] + vertexWeights[3]) < 0.01f); // Assert weights sum equals 1.

        glm::vec4 position = (bones[vertexBoneIDs[0]] * glm::vec4(vertexPosition, 1.0)) * vertexWeights[0];
        position += (bones[vertexBoneIDs[1]] * glm::vec4(vertexPosition, 1.0)) * vertexWeights[1];
        position += (bones[vertexBoneIDs[2]] * glm::vec4(vertexPosition, 1.0)) * vertexWeights[2];
        position += (bones[vertexBoneIDs[3]] * glm::vec4(vertexPosition, 1.0)) * vertexWeights[3];

        glm::vec3 normal = (bonesIT[vertexBoneIDs[0]] * vertexNormal) * vertexWeights[0];
        normal += (bonesIT[vertexBoneIDs[1]] * vertexNormal) * vertexWeights[1];
        normal += (bonesIT[vertexBoneIDs[2]] * vertexNormal) * vertexWeights[2];
        normal += (bonesIT[vertexBoneIDs[3]] * vertexNormal) * vertexWeights[3];

        mVertices[v].position = position;
        mVertices[v].normal = normal;
    }
}
