#include "Material.h"

#include "Dx12RenderVertex.h"
#include "Shader.h"
#include "../Helper/d3dx12.h"
#include "../Helper/PlatformHelpers.h"
#include "../Helper/Paths.h"


void Dx12RenderLearn::Material::CompileShaderIfNot()
{
	auto pathProj = Paths::GetShaderPath("SimplestShader.hlsl");
    if (!pVsByteCode.Get())
    {
        pVsByteCode = Shader::CompileShader(pathProj, Shader::vs, Shader::vs_target,nullptr);
    }
	if (!pPsByteCode.Get())
	{
		pPsByteCode = Shader::CompileShader(pathProj, Shader::ps,Shader::ps_target ,nullptr);
	}
}

