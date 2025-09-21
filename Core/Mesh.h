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
        UINT vertexBufferOffset;
        UINT indicesBufferOffset;
        UINT vertexNum = 0;
        UINT indicesNum = 0;
    public:
        UINT GetVertexNum();
        UINT GetIndicesNum();
        void LoadVertexData(shared_ptr<vector<Dx12RenderVertex>>& pVertexBuffer);
        void LoadIndicesData(shared_ptr<vector<UINT>>& pIndexBuffer);

    };
}