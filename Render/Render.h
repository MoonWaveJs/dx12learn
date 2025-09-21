#pragma once
#include <d3d12.h>
#include <wrl.h>
namespace Dx12RenderLearn
{
    using namespace Microsoft::WRL;
    struct RenderContext
    {
    public:
        ComPtr<ID3D12Device> pDevice;
        ComPtr<ID3D12GraphicsCommandList> pCommandList;
        DXGI_FORMAT mRTFormat;
        DXGI_FORMAT mDSFormat;
        DXGI_SAMPLE_DESC mMultipleSample;
		UINT mNumRenderTarget = 2;
        UINT64 mCbvUavDescriptorSize;
        UINT Width;
		UINT Height;
    };

    struct RenderBufferData
    {
        
    };
}