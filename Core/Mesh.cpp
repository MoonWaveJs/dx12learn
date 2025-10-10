#include "Mesh.h"
#include <DirectXColors.h>
#include "../Helper/BufferHelper.h"
#include "../Helper/Paths.h"
#include "nlohmann/json.hpp"
#include <fstream>

Dx12RenderLearn::Mesh::Mesh(std::string& meshPath)
{
	this->meshPath = meshPath;
	vertexData = std::make_shared<std::vector<Dx12RenderVertex>>();
	indexData = std::make_shared<std::vector<UINT>>();
    LoadMeshData();
}

Dx12RenderLearn::Mesh::~Mesh()
{

}
#pragma region Unused


void Dx12RenderLearn::Mesh::ProcessMesh(aiMesh* mesh, const aiScene* scene, MeshSection& section, aiMatrix4x4 mat)
{
    XMFLOAT4 testColor;
    testColor.x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    testColor.y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    testColor.z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    testColor.w = 1.0f;
    aiVector3D trans, scale, rotate;
    mat.Decompose(scale, rotate, trans);
    // Walk through each of the mesh's vertices
    for (UINT i = 0; i < mesh->mNumVertices; i++) 
    {
        XMFLOAT3 pos;
        auto v = mesh->mVertices[i];

        v *= scene->mRootNode->mTransformation;
        v -= aiVector3D(trans.x, trans.y, trans.z);

        v /= 100.0f;

        pos.x = v.x;
        pos.y = v.y;
        pos.z = v.z;

		XMFLOAT4 color(1,1,1,1);
		color = testColor;

        if (mesh->GetNumColorChannels() > 0)
        {
			color.x = mesh->mColors[0][i].r;
			color.y = mesh->mColors[0][i].g;
			color.z = mesh->mColors[0][i].b;
            color.w = mesh->mColors[0][i].a;
        }

        XMFLOAT3 normal(0,1,0);
        // TODO : load normal

        XMFLOAT4 tex0(0,0,0,0);
        
        if (mesh->HasTextureCoords(0))
        {
            tex0.x = (float)mesh->mTextureCoords[0][i].x;
            tex0.y = (float)mesh->mTextureCoords[0][i].y;
        }
        if( mesh->HasTextureCoords(1))
        {
            tex0.z = (float)mesh->mTextureCoords[1][i].x;
            tex0.w = (float)mesh->mTextureCoords[1][i].y;
		}

        Dx12RenderVertex vertex(pos, normal, color, tex0);
        vertexData->push_back(vertex);
    }
    section.vertexNum = mesh->mNumVertices;

	UINT indexNum = 0;
    for (UINT i = 0; i < mesh->mNumFaces; i++) 
    {
        aiFace face = mesh->mFaces[i];
		indexNum += face.mNumIndices;
        for (UINT j = 0; j < face.mNumIndices; j++)
            indexData->push_back(face.mIndices[j]);
    }
    section.indexNum = indexNum;
}

void Dx12RenderLearn::Mesh::ProcessNode(aiNode* node, const aiScene* scene, aiMatrix4x4 mat)
{
    mat *= node->mTransformation;
	aiVector3D trans, scale,rotate;
    node->mTransformation.Decompose(scale, rotate, trans);
    for (UINT i = 0; i < node->mNumMeshes; i++) 
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        MeshSection section;
#ifdef _DEBUG
        std::string matName = scene->mMaterials[mesh->mMaterialIndex]->GetName().C_Str();
		std::string nodeName = node->mName.C_Str();

		section.sectionName = nodeName + "_" + matName;
#endif
        
        ProcessMesh(mesh, scene,section,mat);
        sections.push_back(section);
    }

    for (UINT i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene,mat);
    }
}
#pragma endregion

void Dx12RenderLearn::Mesh::LoadMeshData()
{
#pragma region Unused

    //Assimp::Importer importer;
    //auto pAssimpScene = importer.ReadFile(meshPath.data(), 
    //    aiProcess_ConvertToLeftHanded |// |aiProcess_JoinIdenticalVertices|
    //    aiProcess_Triangulate
    //    );

    //ProcessNode(pAssimpScene->mRootNode, pAssimpScene,pAssimpScene->mRootNode->mTransformation);

#pragma endregion

    std::filesystem::path sceneFileFolder = Paths::GetProjFolder();
    std::ifstream sceneFileStream(sceneFileFolder / meshPath.data());
    if (sceneFileStream)
    {
        std::string contents((std::istreambuf_iterator<char>(sceneFileStream)), std::istreambuf_iterator<char>());
        nlohmann::json meshJson = nlohmann::json::parse(contents);
        for (auto& p : meshJson["SubMeshInfo"]) 
        {
            MeshSection section;
            p.at("vertexCount").get_to(section.vertexNum);
			p.at("indexCount").get_to(section.indexNum);
#ifdef _DEBUG
			p.at("name").get_to(section.sectionName);
#endif
			sections.push_back(section);
        }
        XMFLOAT4 testColor;
        testColor.x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        testColor.y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        testColor.z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        testColor.w = 1.0f;
        for(int i = 0; i < meshJson["Position"].size();i++)
        {
            XMFLOAT3 position;
            XMFLOAT3 normal;
            XMFLOAT4 color;
            XMFLOAT4 textureCoord0;

			meshJson["Position"][i].at("x").get_to(position.x) * 0.01;
			meshJson["Position"][i].at("y").get_to(position.y) * 0.01;
			meshJson["Position"][i].at("z").get_to(position.z) * 0.01;

            if (meshJson["Normal"].size() > i)
            {
                meshJson["Normal"][i].at("x").get_to(normal.x);
                meshJson["Normal"][i].at("y").get_to(normal.y);
                meshJson["Normal"][i].at("z").get_to(normal.z);
            }
            else
            {
                normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
            }

            if (meshJson["Color"].size() > i)
            {
                meshJson["Color"][i].at("x").get_to(color.x);
                meshJson["Color"][i].at("y").get_to(color.y);
                meshJson["Color"][i].at("z").get_to(color.z);
                meshJson["Color"][i].at("w").get_to(color.w);
            }
            else
            {
                color = testColor;
			}

            if (meshJson["TexCoord0"].size() > i)
            {
                meshJson["TexCoord0"][i].at("x").get_to(textureCoord0.x);
                meshJson["TexCoord0"][i].at("y").get_to(textureCoord0.y);
                meshJson["TexCoord0"][i].at("z").get_to(textureCoord0.z);
                meshJson["TexCoord0"][i].at("w").get_to(textureCoord0.w);
            }
            else
            {
                textureCoord0 = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
            }
            Dx12RenderVertex vertex(position, normal, color, textureCoord0);
            vertexData->push_back(vertex);
        }

        for (int i = 0; i < meshJson["SubMeshIndices"].size(); i++)
        {
            UINT index = 0;
            auto subMeshindices = meshJson["SubMeshIndices"][i];
            for (int j = 0; j < subMeshindices.size(); j++)
            {
                subMeshindices[j].get_to(index);
                indexData->push_back(index);
			}
		}
    }
}
