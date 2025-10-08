#pragma once
#include <memory>
#include <vector>
#include <wrl/client.h>  
#include "Dx12RenderVertex.h"
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace Dx12RenderLearn
{
    struct MeshSection
    {
        UINT vertexNum;
        UINT indexNum;
#ifdef _DEBUG
        std::string sectionName;
#endif
    };

    class Mesh
    {
    private:

        std::string meshPath;

        std::vector<MeshSection> sections;
        std::shared_ptr<std::vector<Dx12RenderVertex>> vertexData;
        std::shared_ptr<std::vector<UINT>> indexData;

    private:
        void ProcessNode(aiNode* node, const aiScene* scene, aiMatrix4x4 mat);
        void ProcessMesh(aiMesh* mesh, const aiScene* scene,MeshSection& section, aiMatrix4x4 mat);

        void LoadMeshData();

    public:
        Mesh(std::string& meshPath);
        ~Mesh();

        std::shared_ptr<std::vector<Dx12RenderVertex>> GetVertexData() { return vertexData; };
        std::shared_ptr<std::vector<UINT>> GetIndexData() { return indexData; };
        
        std::vector<MeshSection> GetSections() { return sections; };
    };
}