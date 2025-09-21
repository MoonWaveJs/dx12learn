#pragma once
#include <memory>
#include <wrl/client.h>
#include "BufferHelper.h"
#include "d3dx12.h"

using namespace Microsoft::WRL;
namespace Dx12RenderLearn 
{
    class BufferHelper
    {
    public:
        static void CreateDefaultBuffer(const ComPtr<ID3D12Device>& pDevice, const ComPtr<ID3D12GraphicsCommandList>& pCommandList,
            const void* pBuffer, UINT64 bufferByteSize, ComPtr<ID3D12Resource>& uploadBuffer,
            ComPtr<ID3D12Resource>& defaultBuffer);

        static void CreateUploadBuffer(const ComPtr<ID3D12Device>& pDevice, UINT64 bufferByteSize,
			ComPtr<ID3D12Resource>& uploadBuffer);

		static void UpdateUploadBuffer(ComPtr<ID3D12Resource>& uploadBuffer, const void* pBuffer, UINT64 bufferByteSize);

        static void CreateConstantBuffer(const ComPtr<ID3D12Device>& pDevice, UINT64 bufferByteSize,
            ComPtr<ID3D12Resource>& uploadBuffer);
    };

}
