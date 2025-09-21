#pragma once
#include<d3d12sdklayers.h>
#include <mutex>
#include<wrl/client.h>

#include "Core/RenderPipeline.h"
#include "Helper/CreateDeviceHelper.h"
#include "Helper/DX12LearnWindowDescHelper.h"

namespace Dx12RenderLearn
{
	struct RenderContext;
	using namespace Microsoft::WRL;
	using namespace DirectX;
	class D3dAppIns
	{
	private:
		ComPtr<ID3D12Device> pDevice;
		ComPtr<ID3D12Fence1> pFence1;
		ComPtr<ID3D12CommandQueue> pCommandQueue;
		ComPtr<ID3D12CommandAllocator> pAllocator;
		ComPtr<ID3D12GraphicsCommandList> pCommandList;
		ComPtr<IDXGIFactory> pDXGIFactory;
		ComPtr<IDXGISwapChain> pSwapchain;
		ComPtr<ID3D12DescriptorHeap> pRenderTargetDescriptorHeap;
		ComPtr<ID3D12DescriptorHeap> pDSDescriptorHeap;
		ComPtr<ID3D12Resource>* pRenderbuffer;
		ComPtr<ID3D12Resource> pDSbuffer;
		
		std::shared_ptr<DX12LearnWindowDescHelper> pDx12LearnWindowDescHelper;
		std::mutex m_EventMutex;
		HANDLE m_FenceEventHandle;

		std::shared_ptr <RenderPipeline> pipline;

	public:
		UINT64 mCurrentFence;
		UINT mRtvDescriptorSize;
		UINT mDsvDescriptorSize;
		// underscore access , constant buffer
		UINT64 mCbvUavDescriptorSize;
		UINT mRenderTargetCount;
		UINT mCurrentBuffer {0};

		DXGI_FORMAT mRTFormat { DXGI_FORMAT_R8G8B8A8_UNORM };
		DXGI_FORMAT mDSFormat { DXGI_FORMAT_D24_UNORM_S8_UINT };
		DXGI_SAMPLE_DESC mMultipleSample = {1,0};
		D3D12_RECT mScissorRect;

		UINT mWidth;
		UINT mHeight;
	public:
		D3dAppIns(const std::shared_ptr<DX12LearnWindowDescHelper> pHelper);
		~D3dAppIns();

	public:
		void Init();
		void Draw();
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetCurrentRtvHandle() const;
		D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentDSvHandle() const;
		ID3D12Resource* GetCurrentBackBuffer() const;
		ID3D12Resource* GetCurrentNextBackBuffer() const;
		void FlushCommandQueue();
		std::shared_ptr<RenderContext> GetRenderContext();
	};

}
