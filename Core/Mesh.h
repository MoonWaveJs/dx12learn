#pragma once
#include <memory>
#include <vector>
#include <wrl/client.h>  
#include "Dx12RenderVertex.h"
namespace Dx12RenderLearn
{
    using namespace std;
    using namespace Microsoft::WRL;
    class Mesh
    {
    private:
        // no smart pointer. this buffer life is manage by scene
        UINT vertexBufferOffsets;
        UINT indicesBufferOffsets;


        std::vector<UINT> vertexNums;
        std::vector<UINT> indicesNums;

    public:
        UINT SectionsNum{ 1 };
        std::vector<UINT> GetVertexNums();
        std::vector<UINT> GetIndicesNums();
        void LoadVertexData(shared_ptr<vector<Dx12RenderVertex>>& pVertexBuffer);
        void LoadIndicesData(shared_ptr<vector<UINT>>& pIndexBuffer);

    };
}