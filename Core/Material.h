#pragma once
#include <d3d12.h>
#include <d3dcommon.h>
#include <memory>
#include <string>
#include <wrl/client.h>

using namespace Microsoft::WRL;
using namespace std;

namespace Dx12RenderLearn
{
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
