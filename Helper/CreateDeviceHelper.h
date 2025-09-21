#pragma once
#include <d3d12.h>
#include <wrl/client.h>

#include "DX12LearnWindowDescHelper.h"
#include "intsafe.h"
#include "PlatformHelpers.h"
#include "d3dx12.h"
#include "dxgi1_3.h"
using namespace Microsoft::WRL;

class CreateDeviceHelper
{
public:
    ///暂时使用默认适配器，DXGIAdapter 传入Null
    ///如果指定适配器就多一步， 使用 DXGIFactory 获取一个 DXGIAdapter 穿进去
    ///传入最低 D3D_FEATURE_LEVEL_11_1，其实会最高。md3dDevice->CheckFeatureSupport 检测只是为了后面可以用哪些特性
    static void CreateDevice(ComPtr<ID3D12Device>& ppOutDevice)
    {
        HRESULT hardwareResult = D3D12CreateDevice(nullptr,D3D_FEATURE_LEVEL_11_1,IID_PPV_ARGS(ppOutDevice.GetAddressOf()));
        DirectX::ThrowIfFailed(hardwareResult);
    }

    static void CreateFence(const ComPtr<ID3D12Device>& ppOutDevice,ComPtr<ID3D12Fence1>& mFence,UINT& mRtvDescriptorSize,UINT &mDsvDescriptorSize,
        UINT64 &mCbvUavDescriptorSize)
    {
        HRESULT hardwareResult = ppOutDevice->CreateFence(0,D3D12_FENCE_FLAG_NONE,IID_PPV_ARGS(mFence.GetAddressOf()));
        DirectX::ThrowIfFailed(hardwareResult);
        mRtvDescriptorSize = ppOutDevice ->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        mDsvDescriptorSize = ppOutDevice ->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
        mCbvUavDescriptorSize = ppOutDevice ->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    }

    /// 多个executecommandlist,相当于隐式插了个完整屏障,会中断gpu执行,所有东西都会停止执行.by U*.后期研究
    static void CreateCommandList(const ComPtr<ID3D12Device>& pDevice,ComPtr<ID3D12CommandQueue>& pCommandQueue,ComPtr<ID3D12CommandAllocator>& pAllocator,ComPtr<ID3D12GraphicsCommandList>& pCommandList)
    {
        D3D12_COMMAND_QUEUE_DESC queue_desc;
        queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queue_desc.NodeMask = 0;
        queue_desc.Priority = 0;
        HRESULT hardwareResult = pDevice->CreateCommandQueue(&queue_desc,IID_PPV_ARGS(pCommandQueue.GetAddressOf()));
        DirectX::ThrowIfFailed(hardwareResult);
        HRESULT hardwareResult1 = pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,IID_PPV_ARGS(pAllocator.GetAddressOf()));
        DirectX::ThrowIfFailed(hardwareResult1);
        // nodeMask：对于仅有一个GPU的系统而言，要将此值设为0；对于具有多GPU的系统而言，此节点掩码（node mask）指定的是与所建命令列表相关联的物理GPU。本书中假设我们使用的是单GPU系统。
        // 指定命令列表的渲染流水线初始状态。对于打包技术来说可将此值设为nullptr，另外，此法同样适用于执行命令列表中不含有任何绘制命令，即执行命令列表是为了达到初始化的目的的特殊情况。
        HRESULT hardwareResult2 = pDevice->CreateCommandList(0,D3D12_COMMAND_LIST_TYPE_DIRECT,pAllocator.Get(),nullptr,IID_PPV_ARGS(pCommandList.GetAddressOf()));
        DirectX::ThrowIfFailed(hardwareResult2);
        pCommandList->Close();
    }

    
    static void CreateSwapChain(const ComPtr<ID3D12Device>& pDevice, const std::shared_ptr<DX12LearnWindowDescHelper>& helper,const ComPtr<ID3D12CommandQueue> &pCommandQueue,ComPtr<IDXGIFactory> &pDXGIFactory,ComPtr<IDXGISwapChain> &pSwapchain, const DXGI_FORMAT &format,const DXGI_SAMPLE_DESC& sample_desc )
    {
        pSwapchain.Reset();
        HRESULT hardwareResult = CreateDXGIFactory2(0,IID_PPV_ARGS(pDXGIFactory.GetAddressOf()));
        DirectX::ThrowIfFailed(hardwareResult);
        DXGI_MODE_DESC mode_desc = helper ->GetDXGIModeDesc();
        mode_desc.Format = format;
        DXGI_SWAP_CHAIN_DESC swap_chain_desc = helper ->GetDXGISwapChainDesc();
        swap_chain_desc.BufferDesc = mode_desc;
        swap_chain_desc.SampleDesc = sample_desc;
        auto result = pDXGIFactory->CreateSwapChain(pCommandQueue.Get(),&swap_chain_desc,pSwapchain.GetAddressOf());
        DirectX::ThrowIfFailed(result);

    }

    static void CreateDescriptorHeaps(const ComPtr<ID3D12Device>& pDevice,ComPtr<ID3D12DescriptorHeap>& pRenderTargetDescriptorHeap,ComPtr<ID3D12DescriptorHeap>& pDSDescriptorHeap,UINT renderTargetCount)
    {
        D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_desc;
        rtv_heap_desc.NodeMask = 0;
        rtv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtv_heap_desc.NumDescriptors =renderTargetCount;
        rtv_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        HRESULT hardwareResult  = pDevice->CreateDescriptorHeap(&rtv_heap_desc,IID_PPV_ARGS(pRenderTargetDescriptorHeap.GetAddressOf()));
        DirectX::ThrowIfFailed(hardwareResult);
        D3D12_DESCRIPTOR_HEAP_DESC dsv_heap_desc;
        dsv_heap_desc.NodeMask = 0;
        dsv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        dsv_heap_desc.NumDescriptors =1;
        dsv_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        HRESULT hardwareResult1  = pDevice->CreateDescriptorHeap(&dsv_heap_desc,IID_PPV_ARGS(pDSDescriptorHeap.GetAddressOf()));
        DirectX::ThrowIfFailed(hardwareResult1);
    }

    static void CreateRenderTargetView(const ComPtr<ID3D12Device>& pDevice,ComPtr<ID3D12Resource>* pRenderbuffer,UINT bufferCount,
    const ComPtr<IDXGISwapChain>& pSwapchain,const ComPtr<ID3D12DescriptorHeap>& pRenderTargetDescriptorHeap,UINT mRtvDescriptorSize)
    {
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(pRenderTargetDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
        for (UINT i = 0; i < bufferCount;i++)
        {
            HRESULT hardwareResult = pSwapchain->GetBuffer(i,IID_PPV_ARGS(pRenderbuffer[i].GetAddressOf()));
            DirectX::ThrowIfFailed(hardwareResult);
            pDevice->CreateRenderTargetView(pRenderbuffer[i].Get(),nullptr,rtvHandle);
            rtvHandle.Offset(1,mRtvDescriptorSize);
        }
    }

    static void CreateDSResource(const ComPtr<ID3D12Device>& pDevice, ComPtr<ID3D12Resource>& pDSbuffer, const std::shared_ptr<DX12LearnWindowDescHelper>& helper, const DXGI_FORMAT& format)
    {
        DXGI_SAMPLE_DESC sample_desc;
        sample_desc.Count = 1;
        sample_desc.Quality = 0;

        D3D12_RESOURCE_DESC depthStencilDesc;
        depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        depthStencilDesc.Alignment = 0; // 内存对齐策略
        depthStencilDesc.Width = helper->GetDXGIModeDesc().Width;
        depthStencilDesc.Height = helper->GetDXGIModeDesc().Height;
        depthStencilDesc.DepthOrArraySize = 1;
        depthStencilDesc.MipLevels = 1;
        depthStencilDesc.Format = format;
        depthStencilDesc.SampleDesc = sample_desc;
        depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN; // 纹理布局
        depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

        D3D12_CLEAR_VALUE depthStencilClearValue;
        depthStencilClearValue.DepthStencil.Depth = 1.0f;
        depthStencilClearValue.DepthStencil.Stencil = 0;
        depthStencilClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        CD3DX12_HEAP_PROPERTIES HeapProps(D3D12_HEAP_TYPE_DEFAULT);
        HRESULT hardwareResult = pDevice->CreateCommittedResource(&HeapProps, D3D12_HEAP_FLAG_NONE, &depthStencilDesc, D3D12_RESOURCE_STATE_COMMON, &depthStencilClearValue,
            IID_PPV_ARGS(pDSbuffer.GetAddressOf()));
        DirectX::ThrowIfFailed(hardwareResult);
    }

    static void CreateDepthStencilView(const ComPtr<ID3D12Device>& pDevice, const ComPtr<ID3D12Resource>& pDSbuffer, const ComPtr<ID3D12DescriptorHeap>& pDSDescriptorHeap,
        const ComPtr<ID3D12GraphicsCommandList>& pCommandList)
    {
        // D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc; //创建资源的时候已经指定，无需再次指定
        pDevice->CreateDepthStencilView(pDSbuffer.Get(),nullptr, pDSDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
    }

    static void SettingViewPort(const std::shared_ptr<DX12LearnWindowDescHelper>& helper, const ComPtr<ID3D12GraphicsCommandList>& pCommandList, D3D12_RECT& outScissorRect)
    {
        D3D12_VIEWPORT viewport;
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = helper->GetDXGIModeDesc().Width;
        viewport.Height = helper->GetDXGIModeDesc().Height;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        pCommandList->RSSetViewports(1, &viewport);

        outScissorRect = {
                        0,
                        0,
                        static_cast<LONG>(helper->GetDXGIModeDesc().Width),
                        static_cast<LONG>(helper->GetDXGIModeDesc().Height)
        };
        pCommandList->RSSetScissorRects(1, &outScissorRect);
    }

};
