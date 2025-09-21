#pragma once
#include <string>
namespace Dx12RenderLearn
{
    class Paths
    {
    public:
        static std::wstring GetProjFolder()
        {
            // 获取当前工作目录
            wchar_t* cwd = _wgetcwd(nullptr, 0);
            if (cwd)
            {
                std::wstring path(cwd);
                free(cwd); // 释放内存
                return path;
            }
            return L"";
        }

        static std::wstring GetShaderFolder() 
        {
            auto proj = GetProjFolder();
            proj.reserve(proj.size() + 6);
            proj += L"\\Shader";
            return proj;
        }

        static std::wstring GetShaderPath(std::string shaderName)
        {
            auto proj = GetProjFolder();
            proj.reserve(proj.size() + 6 + shaderName.size() + 1);
            proj += L"\\Shader\\";
            std::wstring wShaderName(shaderName.begin(), shaderName.end());
            proj += wShaderName;
            return proj;
        }

    };
}