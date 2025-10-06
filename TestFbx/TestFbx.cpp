#pragma once
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

int main()
{
    //GetFbxLoader()->FnGetVertexGroup();
    //std::cout << "Hello World!/n";
    using namespace Assimp;
    Importer importer;
    std::string filename("C:/jiangshang/dx12learn/Assets/UnityTechnologies/Basic Asset Pack Interior/Models/BedDouble.FBX");
    auto pAssimpScene = importer.ReadFile(filename.data(),
        aiProcess_Triangulate |             // 将多边形拆分
        aiProcess_ImproveCacheLocality |    // 改善缓存局部性
        aiProcess_SortByPType);             // 按图元顶点数排序用于移除非三角形图元
    //printf(importer.GetErrorString());
    std::cout << (pAssimpScene->mMeshes[0]->mNumFaces);
}
