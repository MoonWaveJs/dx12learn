#include "DX12LearnWindowDescHelper.h"


DXGI_MODE_DESC DX12LearnWindowDescHelperEx::GetDXGIModeDesc()
{
    DXGI_MODE_DESC ModeDesc;
    ModeDesc.Width = Width;
    ModeDesc.Height = Height;
    ModeDesc.RefreshRate = {90,1 };
    // ModeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    ModeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    ModeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    return ModeDesc;
}

DXGI_SWAP_CHAIN_DESC DX12LearnWindowDescHelperEx::GetDXGISwapChainDesc()
{
    DXGI_SWAP_CHAIN_DESC ChainDesc;
    // ChainDesc.SampleDesc = { 1,0 };
    ChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    ChainDesc.BufferCount = 2;
    ChainDesc.OutputWindow = WinIns;
    ChainDesc.Windowed = TRUE;
    ChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    ChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    return ChainDesc;
}
