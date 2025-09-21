#include "BufferHelper.h"
#include "PlatformHelpers.h"

void Dx12RenderLearn::BufferHelper::CreateDefaultBuffer(const ComPtr<ID3D12Device>& pDevice,const ComPtr<ID3D12GraphicsCommandList>& pCommandList,
                                         const void* pBuffer, UINT64 bufferByteSize, ComPtr<ID3D12Resource>& uploadBuffer,
                                         ComPtr<ID3D12Resource>& defaultBuffer)
{
    // 先创建default buffer
    CD3DX12_HEAP_PROPERTIES HeapProps(D3D12_HEAP_TYPE_DEFAULT);
    CD3DX12_RESOURCE_DESC ResDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferByteSize);
    HRESULT hardwareResult = pDevice->CreateCommittedResource(&HeapProps,D3D12_HEAP_FLAG_NONE,&ResDesc,D3D12_RESOURCE_STATE_COMMON,nullptr,IID_PPV_ARGS(defaultBuffer.GetAddressOf()));
    DirectX::ThrowIfFailed(hardwareResult);
    
    // 将Default buffer设置为复制的目的地状态
    CD3DX12_RESOURCE_BARRIER Transition = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(),D3D12_RESOURCE_STATE_COMMON,D3D12_RESOURCE_STATE_COPY_DEST);
    pCommandList->ResourceBarrier(1,&Transition);

    CreateUploadBuffer(pDevice, bufferByteSize, uploadBuffer);

    D3D12_SUBRESOURCE_DATA Subresource_data;
    Subresource_data.pData = pBuffer;
    Subresource_data.RowPitch = bufferByteSize;
    Subresource_data.SlicePitch = bufferByteSize;

    //将upload buffer 复制到 default buffer
    UpdateSubresources(pCommandList.Get(),defaultBuffer.Get(),uploadBuffer.Get(),0,0,1,&Subresource_data);

    //将Default buffer转回 D3D12_RESOURCE_STATE_GENERIC_READ
    Transition = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(),D3D12_RESOURCE_STATE_COPY_DEST,D3D12_RESOURCE_STATE_GENERIC_READ);
    pCommandList->ResourceBarrier(1,&Transition);

	// TODO: relase the upload buffer after command list is executed
}

void Dx12RenderLearn::BufferHelper::CreateUploadBuffer(const ComPtr<ID3D12Device>& pDevice,UINT64 bufferByteSize, ComPtr<ID3D12Resource>& uploadBuffer)
{
    CD3DX12_RESOURCE_DESC ResDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferByteSize);
    // 再创建upload buffer
    CD3DX12_HEAP_PROPERTIES HeapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    HRESULT hardwareResult = pDevice->CreateCommittedResource(&HeapProps, D3D12_HEAP_FLAG_NONE, &ResDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(uploadBuffer.GetAddressOf()));
    DirectX::ThrowIfFailed(hardwareResult);
}

void Dx12RenderLearn::BufferHelper::UpdateUploadBuffer(ComPtr<ID3D12Resource>& uploadBuffer, const void* pBuffer, UINT64 bufferByteSize)
{
    BYTE* mMappedData = nullptr;
    uploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData));
    memcpy(mMappedData, pBuffer, bufferByteSize);
	uploadBuffer->Unmap(0, nullptr);
}

void Dx12RenderLearn::BufferHelper::CreateConstantBuffer(const ComPtr<ID3D12Device>& pDevice, UINT64 bufferByteSize, ComPtr<ID3D12Resource>& uploadBuffer)
{
	CreateUploadBuffer(pDevice, bufferByteSize, uploadBuffer);
}
