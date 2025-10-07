#pragma once
#include <d3d12.h>
#include <d3dcommon.h>
#include <memory>
#include <string>
#include <wrl/client.h>

namespace Dx12RenderLearn
{
    using namespace Microsoft::WRL;
    using namespace std;
    class Material
    {
    public:
        ComPtr<ID3DBlob> pVsByteCode;
        ComPtr<ID3DBlob> pPsByteCode;
        void CompileShaderIfNot();
    private:
    private:
    };
 
}
