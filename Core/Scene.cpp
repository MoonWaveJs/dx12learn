#include "Scene.h"

#include <fstream>

#include "StaticMeshEntity.h"
#include "../Helper/BufferHelper.h"
using namespace Dx12RenderLearn;

vector<std::shared_ptr<Model>> Dx12RenderLearn::Scene::CollectModels(vector<shared_ptr<StaticMeshEntity>> entities,UINT& outVertexCount,UINT& outIndicesCount)
{
    vector<std::shared_ptr<Model>> models = vector<std::shared_ptr<Model>>();

    for (shared_ptr<StaticMeshEntity> entity : entities)
    {
        auto mesh = entity->model->mesh;
		outVertexCount += mesh->GetVertexData()->size();
		outIndicesCount += mesh->GetIndexData()->size();
        models.push_back(entity->model);
    }
    return std::move(models);
}

vector<std::shared_ptr<StaticMeshEntity>> Dx12RenderLearn::Scene::CollectEntites()  
{  
   vector<std::shared_ptr<StaticMeshEntity>> entities;

   string path = "E:/Project/dx12-learn/Assets/UnityTechnologies/Basic Asset Pack Interior/Models/BedDouble.FBX";
   std::shared_ptr<StaticMeshEntity> entity1 = std::make_shared<StaticMeshEntity>();
   entity1->model = make_shared<Model>(path);
   entities.push_back(entity1);


   std::shared_ptr<StaticMeshEntity> entity = std::make_shared<StaticMeshEntity>();  
   path = "E:/Project/dx12-learn/Assets/UnityTechnologies/Basic Asset Pack Interior/Models/BaseCharacter.FBX";
   entity->model = make_shared<Model>(path);
   entity->SetScale(100, 100, 100);
   entity->SetPosition(0, 0, 200);
   entities.push_back(entity);

   return entities; 
}

Dx12RenderLearn::Scene::Scene(const string& path, std::shared_ptr<RenderContext> conext)
{
	renderContext = conext;

    /// collect render item
    UINT staticMeshVertexNum = 0;
    UINT staticMeshIndicesNum = 0;
    auto collectEntities =  CollectEntites();
    sceneEntities = make_shared<vector<shared_ptr<StaticMeshEntity>>>(collectEntities);
    CreatePerobjectBufferView();
    auto models = CollectModels(collectEntities,staticMeshVertexNum,staticMeshIndicesNum);
    
    renderMeshVertices = make_shared<vector<Dx12RenderVertex>>();
    renderMeshVertices->reserve(staticMeshVertexNum);
    renderMeshIndices = make_shared<vector<UINT>>();
    renderMeshIndices->reserve(staticMeshIndicesNum);

    for (auto& model : models)
    {
        for (int i = 0; i < model->materials.size(); i++)
        {
            model->materials[i]->CompileShaderIfNot();
			renderMeshVertices->insert(renderMeshVertices->end(), model->mesh->GetVertexData()->begin(), model->mesh->GetVertexData()->end());
			renderMeshIndices->insert(renderMeshIndices->end(), model->mesh->GetIndexData()->begin(), model->mesh->GetIndexData()->end());
        }
    }

    UINT bufferSize = renderMeshVertices->size() * sizeof(Dx12RenderVertex);
    Dx12RenderLearn::BufferHelper::CreateDefaultBuffer(renderContext->pDevice,renderContext->pCommandList,&renderMeshVertices->at(0)
     ,bufferSize,uploadVertexBuffer,defaultVertexBuffer);
    staticVertexBufferView = make_shared<D3D12_VERTEX_BUFFER_VIEW>();
    staticVertexBufferView->BufferLocation = defaultVertexBuffer ->GetGPUVirtualAddress();
    staticVertexBufferView->SizeInBytes = bufferSize;
    staticVertexBufferView->StrideInBytes = sizeof(Dx12RenderVertex);
    renderContext->pCommandList->IASetVertexBuffers(0,1,staticVertexBufferView.get());

    bufferSize = renderMeshIndices->size() * sizeof(UINT);
    BufferHelper::CreateDefaultBuffer(renderContext->pDevice,renderContext->pCommandList,&renderMeshIndices->at(0)
     ,bufferSize,uploadIndexBuffer,defaultIndexBuffer);
    staticIndexBufferView =  make_shared<D3D12_INDEX_BUFFER_VIEW>();
    staticIndexBufferView->Format = DXGI_FORMAT_R32_UINT;
    staticIndexBufferView->BufferLocation = defaultIndexBuffer->GetGPUVirtualAddress();
    staticIndexBufferView->SizeInBytes = bufferSize;
    renderContext->pCommandList ->IASetIndexBuffer(staticIndexBufferView.get());
}

void Dx12RenderLearn::Scene::CreatePerobjectBufferView()
{
    D3D12_DESCRIPTOR_HEAP_DESC perObjectBufferHeapDesc;
    perObjectBufferHeapDesc.NumDescriptors = sceneEntities->size();
    perObjectBufferHeapDesc.NodeMask = 0;
    perObjectBufferHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    perObjectBufferHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    renderContext->pDevice->CreateDescriptorHeap(&perObjectBufferHeapDesc, IID_PPV_ARGS(&objectParamBufferHeap));

    CD3DX12_CPU_DESCRIPTOR_HANDLE opHandle(objectParamBufferHeap->GetCPUDescriptorHandleForHeapStart());

    for (auto entity : *sceneEntities)
    {
        BufferHelper::CreateConstantBuffer(renderContext->pDevice, sizeof(EntityRenderBuffer), entity->uploadConstantBuffer);
        D3D12_CONSTANT_BUFFER_VIEW_DESC objectBufferViewDesc;
        objectBufferViewDesc.BufferLocation = entity->uploadConstantBuffer->GetGPUVirtualAddress();
        objectBufferViewDesc.SizeInBytes = sizeof(EntityRenderBuffer);
        renderContext->pDevice->CreateConstantBufferView(&objectBufferViewDesc, opHandle);
		opHandle = opHandle.Offset(1, renderContext->mCbvUavDescriptorSize);
    }
}
