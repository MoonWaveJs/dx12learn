#pragma once  
#include <wrl/client.h>  
#include <d3d12.h>  
#include <memory>  

#include "Material.h"  
#include "../Core/Scene.h"
#include "../Render/Render.h"
#include "../Helper/d3dx12.h"

using namespace Microsoft::WRL;  
using namespace std;  
namespace Dx12RenderLearn  
{  

   class RenderPipeline  
   {  
   private:  
       std::shared_ptr<Scene> currentScene;  
       std::shared_ptr<RenderContext> renderContext;  

       ComPtr<ID3D12RootSignature> root_signature{ nullptr };
       ComPtr<ID3D12PipelineState> PSO; //TODO:only one for now  
   public:  
       RenderPipeline(std::shared_ptr<RenderContext> conext);  
       void LoadScene(const string& path);  
       void RenderStaticScene();  

   private:  
       ComPtr<ID3D12RootSignature> GetOrCreateSignature(ComPtr<ID3D12Device> device);
       void CreateRootSignature(ComPtr<ID3D12Device> device);
       void CreatePSO(ComPtr<ID3D12PipelineState>& pso, const std::shared_ptr<Material> mat);  
   };  
}
