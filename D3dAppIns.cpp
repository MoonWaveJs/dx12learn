#include "D3dAppIns.h"
#include <DirectXColors.h>

Dx12RenderLearn::D3dAppIns::D3dAppIns(const std::shared_ptr<DX12LearnWindowDescHelper> pHelper)
{
    this->pDx12LearnWindowDescHelper = pHelper;
    mCurrentFence = 0;
    mRenderTargetCount = pHelper->GetDXGISwapChainDesc().BufferCount;
    pRenderbuffer = new ComPtr<ID3D12Resource>[mRenderTargetCount];
#if defined(DEBUG) || defined(_DEBUG) 
    {
        ComPtr<ID3D12Debug> debugController;
        ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
        debugController->EnableDebugLayer();
    }
#endif
    
    CreateDeviceHelper::CreateDevice(pDevice);
    CreateDeviceHelper::CreateFence(pDevice,pFence1,mRtvDescriptorSize,mDsvDescriptorSize,mCbvUavDescriptorSize);
    CreateDeviceHelper::CreateCommandList(pDevice,pCommandQueue,pAllocator,pCommandList);
    CreateDeviceHelper::CreateSwapChain(pDevice,pDx12LearnWindowDescHelper,pCommandQueue,pDXGIFactory,pSwapchain,mRTFormat,mMultipleSample);
    CreateDeviceHelper::CreateDescriptorHeaps(pDevice,pRenderTargetDescriptorHeap,pDSDescriptorHeap,mRenderTargetCount);
    CreateDeviceHelper::CreateRenderTargetView(pDevice,pRenderbuffer,mRenderTargetCount,pSwapchain,pRenderTargetDescriptorHeap,mRtvDescriptorSize);
    CreateDeviceHelper::CreateDSResource(pDevice, pDSbuffer, pDx12LearnWindowDescHelper,mDSFormat);
    CreateDeviceHelper::CreateDepthStencilView(pDevice, pDSbuffer, pDSDescriptorHeap, pCommandList);
    mWidth = pHelper->GetDXGIModeDesc().Width;
	mHeight = pHelper->GetDXGIModeDesc().Height;
}

Dx12RenderLearn::D3dAppIns::~D3dAppIns()
{
}

void Dx12RenderLearn::D3dAppIns::Init()
{
    HRESULT hardwareResult = pAllocator->Reset();
    ThrowIfFailed(hardwareResult);
    hardwareResult = pCommandList->Reset(pAllocator.Get(), nullptr);
    ThrowIfFailed(hardwareResult);
    CD3DX12_RESOURCE_BARRIER Barrier1 = CD3DX12_RESOURCE_BARRIER::Transition(pDSbuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
    pCommandList->ResourceBarrier(1, &Barrier1);
    pipline = std::make_shared<RenderPipeline>(GetRenderContext());
	pipline->LoadScene("");
	
    hardwareResult = pCommandList->Close();
    ThrowIfFailed(hardwareResult);
    ID3D12CommandList* ppCommandList[] = { pCommandList.Get() };
    pCommandQueue->ExecuteCommandLists(_countof(ppCommandList), ppCommandList);
    FlushCommandQueue();

}

void Dx12RenderLearn::D3dAppIns::Draw()
{
    HRESULT hardwareResult = pAllocator->Reset();
    ThrowIfFailed(hardwareResult);
    hardwareResult = pCommandList->Reset(pAllocator.Get(), nullptr);
    ThrowIfFailed(hardwareResult);
    CD3DX12_RESOURCE_BARRIER TempBarrier = CD3DX12_RESOURCE_BARRIER::Transition(GetCurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    pCommandList->ResourceBarrier(1,&TempBarrier);

    CreateDeviceHelper::SettingViewPort(pDx12LearnWindowDescHelper, pCommandList,mScissorRect);

    XMVECTORF32 Color{.0f,.0f,.0f,.0f};
    pCommandList->ClearRenderTargetView(GetCurrentRtvHandle(), Color, 0, nullptr);
    pCommandList->ClearDepthStencilView(GetCurrentDSvHandle(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
    CD3DX12_CPU_DESCRIPTOR_HANDLE CurrentRtvHandle = GetCurrentRtvHandle();
    D3D12_CPU_DESCRIPTOR_HANDLE CurrentDSvHandle = GetCurrentDSvHandle();
    pCommandList->OMSetRenderTargets(1,&CurrentRtvHandle,true,&CurrentDSvHandle);

    pipline->RenderStaticScene();

    CD3DX12_RESOURCE_BARRIER Barrier = CD3DX12_RESOURCE_BARRIER::Transition(GetCurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    pCommandList->ResourceBarrier(1, &Barrier);
    hardwareResult = pCommandList->Close();
    ThrowIfFailed(hardwareResult);
    ID3D12CommandList* ppCommandList[] = { pCommandList.Get() };
    pCommandQueue->ExecuteCommandLists(_countof(ppCommandList), ppCommandList);
    hardwareResult = pSwapchain->Present(0, 0);
    ThrowIfFailed(hardwareResult);
    mCurrentBuffer = (mCurrentBuffer + 1) % mRenderTargetCount;
    FlushCommandQueue();
}

ID3D12Resource* Dx12RenderLearn::D3dAppIns::GetCurrentBackBuffer() const
{
    return pRenderbuffer[mCurrentBuffer].Get();
}

CD3DX12_CPU_DESCRIPTOR_HANDLE Dx12RenderLearn::D3dAppIns::GetCurrentRtvHandle() const
{
    return CD3DX12_CPU_DESCRIPTOR_HANDLE(pRenderTargetDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),mCurrentBuffer,mRtvDescriptorSize);
}

D3D12_CPU_DESCRIPTOR_HANDLE Dx12RenderLearn::D3dAppIns::GetCurrentDSvHandle() const
{
    return pDSDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
}

ID3D12Resource* Dx12RenderLearn::D3dAppIns::GetCurrentNextBackBuffer() const
{
    return pRenderbuffer[(mCurrentBuffer+1)% mRenderTargetCount].Get();
}

void Dx12RenderLearn::D3dAppIns::FlushCommandQueue()
{
    mCurrentFence++;

    HRESULT hardwareResult = pCommandQueue->Signal(pFence1.Get(), mCurrentFence);
    ThrowIfFailed(hardwareResult);
    if (pFence1->GetCompletedValue() < mCurrentFence) 
    {
        m_FenceEventHandle = CreateEvent(nullptr, false, false, nullptr);
        assert(m_FenceEventHandle != NULL);
        //std::lock_guard<std::mutex> LockGuard(m_EventMutex);
        hardwareResult = pFence1->SetEventOnCompletion(mCurrentFence, m_FenceEventHandle);
        ThrowIfFailed(hardwareResult);
        WaitForSingleObject(m_FenceEventHandle, INFINITE);

        ResetEvent(m_FenceEventHandle);
        CloseHandle(m_FenceEventHandle);
    }
}

std::shared_ptr<Dx12RenderLearn::RenderContext> Dx12RenderLearn::D3dAppIns::GetRenderContext()
{
    auto context = std::make_shared<RenderContext>();
    context->pDevice = pDevice;
    context->pCommandList = pCommandList;
	context->mRTFormat = mRTFormat;
    context->mDSFormat = mDSFormat;
	context->mMultipleSample = mMultipleSample;
	context->mNumRenderTarget = mRenderTargetCount;
	context->mCbvUavDescriptorSize = mCbvUavDescriptorSize;
	context->Width = mWidth;
	context->Height = mHeight;
    return context;
}
