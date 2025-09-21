#pragma once
#include <list>
#include <string>
#include <wrl/client.h>
#include "Dx12RenderVertex.h"
#include "Model.h"
#include "StaticMeshEntity.h"
#include "../Render/Render.h" 
#include <DirectXPackedVector.h>

using namespace std;
using namespace Microsoft::WRL;

namespace Dx12RenderLearn
{
    using namespace  DirectX;
    struct GlobalParamBuffer
    {
        XMMATRIX gViewProj;
        XMFLOAT3 gCameraPos;
		char padding[256 - sizeof(XMMATRIX) - sizeof(XMFLOAT3)];
    };

    class Scene
    {
    public:
        // 先整一个立方体
        vector<std::shared_ptr<Model>> CollectModels(vector<shared_ptr<StaticMeshEntity>> entities,UINT& outVertexCount,UINT& outIndicesCount);
        vector<std::shared_ptr<StaticMeshEntity>> CollectEntites();
        /// 1、init model , mesh, pipeline vertex buffer
        Scene(const string& path, std::shared_ptr<RenderContext> conext);

        std::shared_ptr<RenderContext> renderContext;
        shared_ptr<vector<Dx12RenderVertex>> renderMeshVertices;
        shared_ptr<vector<UINT>> renderMeshIndices;
        std::shared_ptr<vector<std::shared_ptr<StaticMeshEntity>>> sceneEntities;

        shared_ptr<D3D12_INDEX_BUFFER_VIEW> staticIndexBufferView;
        shared_ptr<D3D12_VERTEX_BUFFER_VIEW> staticVertexBufferView;
        // GlobalParamBuffer
		ComPtr<ID3D12DescriptorHeap> globalParamBufferHeap;
        ComPtr<ID3D12Resource> uploadGlobalConstantBuffer;
		// perObject buffer
        ComPtr<ID3D12DescriptorHeap> objectParamBufferHeap;
    private:
        // delete on default buffer copy finish.
        Microsoft::WRL::ComPtr<ID3D12Resource> uploadVertexBuffer;
        ComPtr<ID3D12Resource> defaultVertexBuffer;

        // delete on default buffer copy finish.
        ComPtr<ID3D12Resource> uploadIndexBuffer;
        ComPtr<ID3D12Resource> defaultIndexBuffer;

        void CreatePerobjectBufferView();
    };
}
