#pragma once
#include <memory>
#include <vector>
#include <wrl/client.h>  
#include "Dx12RenderVertex.h"
#include <string>
namespace Dx12RenderLearn
{
    using namespace std;
    using namespace Microsoft::WRL;

    struct MeshInfo 
    {
        UINT SectionsNum{ 1 };
        vector<UINT> vertexNums;
        vector<UINT> indicesNums;
    };

    class Mesh
    {
    private:
        // no smart pointer. this buffer life is manage by scene
        UINT vertexBufferOffsets;
        UINT indicesBufferOffsets;

        MeshInfo meshInfo;
		string meshPath;

    public:
        Mesh(std::string meshPath);
        vector<UINT> GetVertexNums();
        vector<UINT> GetIndicesNums();
        void LoadVertexData(shared_ptr<vector<Dx12RenderVertex>>& pVertexBuffer);
        void LoadIndicesData(shared_ptr<vector<UINT>>& pIndexBuffer);

    };
}