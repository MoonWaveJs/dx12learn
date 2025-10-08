#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <DirectXPackedVector.h>

namespace Dx12RenderLearn
{
    using namespace Microsoft::WRL;
    using namespace  DirectX;

    struct EntityRenderBuffer
    {
        XMMATRIX mTransform = XMMatrixIdentity();
		char padding[256 - sizeof(XMMATRIX)];
    };

    class IEntity
    {
    private:
		ComPtr<ID3D12Resource> mTransformBuffer;
		void InitTransformBuffer();


    public:
        bool isDirty{ false };
        XMMATRIX mTransform = XMMatrixIdentity();
        ComPtr<ID3D12Resource> uploadConstantBuffer;
    public:
        void SetScale(float x, float y, float z);
        void SetPosition(float x, float y, float z);
        void SetRotation(float x, float y, float z, float w);
    };
}