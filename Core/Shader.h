#include <d3dcompiler.h>
#include "../Log/DxLog.h"
#include <wrl.h>
#include "../Helper/PlatformHelpers.h"
namespace Dx12RenderLearn
{
    class Shader
    {
    public:
        static const char* vs;
        static const char* vs_target;
        static const char* ps;
        static const char* ps_target;
        static Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(const std::wstring& file,const char* entryPoint, const char* target,const D3D_SHADER_MACRO* shader_macro)
        {
            UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
            compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
            Microsoft::WRL::ComPtr<ID3DBlob> byteCode = nullptr;
            Microsoft::WRL::ComPtr<ID3DBlob> errors = nullptr;
            HRESULT result = D3DCompileFromFile(file.c_str(),shader_macro,D3D_COMPILE_STANDARD_FILE_INCLUDE,entryPoint, target,compileFlags,0,&byteCode,&errors);
            if (errors != nullptr)
            {
                Dx12RenderLearn::DxLog::Log(std::string(static_cast<char*>(errors->GetBufferPointer())));
            }
            DirectX::ThrowIfFailed(result);
            return byteCode;
        }
    };

    // 删除这两行定义，改为在 Shader.cpp 中定义
    // const char* Shader::vs = "VS";
    // const char* Shader::ps = "PS";
}
