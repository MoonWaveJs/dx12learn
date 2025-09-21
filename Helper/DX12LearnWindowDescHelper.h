#pragma once
#include <dxgi.h>
#include <dxgitype.h>

class DX12LearnWindowDescHelper
{
public:
    virtual ~DX12LearnWindowDescHelper() = default;
    virtual DXGI_MODE_DESC GetDXGIModeDesc() = 0;
    virtual DXGI_SWAP_CHAIN_DESC GetDXGISwapChainDesc() = 0;
};

class DX12LearnWindowDescHelperEx:public DX12LearnWindowDescHelper
{
public:
    HWND WinIns;
    UINT Width;
    UINT Height;
public:
    DX12LearnWindowDescHelperEx(HWND WinIns, UINT Width, UINT Height)
        : WinIns(WinIns), Width(Width), Height(Height) {
    }
     DXGI_MODE_DESC GetDXGIModeDesc() override;
     DXGI_SWAP_CHAIN_DESC GetDXGISwapChainDesc() override;

};
