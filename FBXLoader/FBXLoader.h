#pragma once
#include <string>
#include <fbxsdk.h>
#include <memory>

__declspec(dllexport) struct VertexData
{
	FbxDouble3 Position;
	FbxDouble3 Rotation;
	FbxDouble3 Scale;
	FbxDouble3 Normal;
};

__declspec(dllexport) class FbxLoader
{
public:
    __declspec(dllexport) void FnFbxLoaderInit();
    __declspec(dllexport) void FnOutputScene(std::string fbx);

    __declspec(dllexport) void FnGetVertexGroup();
    
    __declspec(dllexport) void Release();
    ~FbxLoader();
    FbxLoader();

private:
    FbxManager* lSdkManager;
    FbxImporter* lImporter;
private:

};

std::shared_ptr<FbxLoader> _declspec(dllexport) GetFbxLoader();
