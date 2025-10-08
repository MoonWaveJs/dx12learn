#include "Mesh.h"
#include <DirectXColors.h>
#include "../Helper/BufferHelper.h"

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

void Dx12RenderLearn::Mesh::LoadMeshData()
{
    Assimp::Importer importer;
    auto pAssimpScene = importer.ReadFile(meshPath.data(), 
        aiProcess_ConvertToLeftHanded |// |aiProcess_JoinIdenticalVertices|
        aiProcess_Triangulate
        );

    ProcessNode(pAssimpScene->mRootNode, pAssimpScene,pAssimpScene->mRootNode->mTransformation);
}
