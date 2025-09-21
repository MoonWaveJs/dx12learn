#include "RenderPipeline.h"
#include "../Helper/PlatformHelpers.h"
#include "../Helper/BufferHelper.h"

Dx12RenderLearn::RenderPipeline::RenderPipeline(std::shared_ptr<RenderContext> conext)
{
    renderContext = conext;
}

void Dx12RenderLearn::RenderPipeline::LoadScene(const string& path)
{
    currentScene = std::make_shared<Scene>(path, renderContext);
}

void Dx12RenderLearn::RenderPipeline::RenderStaticScene()
{
    ID3D12DescriptorHeap* descriptorHeaps[] = {
        //currentScene->globalParamBufferHeap.Get(),
        currentScene->objectParamBufferHeap.Get()
    };
    renderContext->pCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

    auto signtrue = GetOrCreateSignature(renderContext->pDevice);
    renderContext->pCommandList->SetGraphicsRootSignature(signtrue.Get());

    /*GlobalParamBuffer globalParam;
    BufferHelper::UpdateUploadBuffer(currentScene->uploadGlobalConstantBuffer, &globalParam, sizeof(GlobalParamBuffer));
    renderContext->pCommandList->SetGraphicsRootDescriptorTable(0, currentScene->globalParamBufferHeap->GetGPUDescriptorHandleForHeapStart());*/

    if (currentScene && currentScene->sceneEntities)
    {

        for (const auto& entity : *(currentScene->sceneEntities))
        {
            XMMATRIX Proj = XMMatrixPerspectiveFovLH(0.25f * XM_PI,
                static_cast<float>(renderContext->Width) / renderContext->Height, 1.0f, 1000.0f);

            XMVECTOR pos = XMVectorSet(10,10,10,1.0f);
            XMVECTOR target = XMVectorZero();
            XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
            XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
            
            EntityRenderBuffer entityRenderBuffer;

            entityRenderBuffer.mTransform = entity->mTransform * view *Proj;
			
            BufferHelper::UpdateUploadBuffer(entity->uploadConstantBuffer, &entityRenderBuffer, sizeof(EntityRenderBuffer));

            CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle(currentScene->objectParamBufferHeap->GetGPUDescriptorHandleForHeapStart());
            renderContext->pCommandList->SetGraphicsRootDescriptorTable(1, gpuHandle);
			gpuHandle.Offset(1, renderContext->mCbvUavDescriptorSize);

			auto& model = entity->Model;
            for (int i = 0; i < entity->Model->SectionsNum;i++)
            {
				renderContext->pCommandList->IASetVertexBuffers(0, 1, currentScene->staticVertexBufferView.get());
				renderContext->pCommandList->IASetIndexBuffer(currentScene->staticIndexBufferView.get());

                CreatePSO(PSO, PGETMODELMATRIAL(model, i));
				renderContext->pCommandList->SetPipelineState(PSO.Get());
                renderContext->pCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                // just one model for now,not suitable for multiple
                renderContext->pCommandList->DrawIndexedInstanced(PGETMODELMESH(model,i)->GetIndicesNum(), 1, 0, 0, 0);
            }
            
        }
    }
}

void Dx12RenderLearn::RenderPipeline::CreatePSO(ComPtr<ID3D12PipelineState>& pso, const std::shared_ptr<Material> mat)
{
    D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc;
    ZeroMemory(&pso_desc,sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
    pso_desc.pRootSignature = GetOrCreateSignature(renderContext->pDevice).Get();

    pso_desc.VS = { 
        reinterpret_cast<void*>(mat->pVsByteCode->GetBufferPointer()),
        mat->pVsByteCode->GetBufferSize() 
      };
    pso_desc.PS = { 
        reinterpret_cast<void*>(mat->pPsByteCode->GetBufferPointer()),
        mat->pPsByteCode->GetBufferSize() 
      };
    pso_desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    pso_desc.SampleMask = UINT_MAX;
    pso_desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    pso_desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    pso_desc.InputLayout = Dx12RenderVertex::GetInputLayoutDesc();
    pso_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    pso_desc.NumRenderTargets = renderContext->mNumRenderTarget;
    pso_desc.RTVFormats[0] = renderContext->mRTFormat;
    pso_desc.DSVFormat = renderContext->mDSFormat;
    pso_desc.SampleDesc = renderContext->mMultipleSample;

    auto result = renderContext->pDevice->CreateGraphicsPipelineState(&pso_desc,IID_PPV_ARGS(pso.GetAddressOf()));
    DirectX::ThrowIfFailed(result);
}

void Dx12RenderLearn::RenderPipeline::CreateRootSignature(ComPtr<ID3D12Device> device)
{
    CD3DX12_ROOT_PARAMETER slot_root_parameter[2];
    CD3DX12_DESCRIPTOR_RANGE gParmCbvTable;
    gParmCbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
    slot_root_parameter[0].InitAsDescriptorTable(1, &gParmCbvTable);
    CD3DX12_DESCRIPTOR_RANGE cbvTable;
    cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);
    slot_root_parameter[1].InitAsDescriptorTable(1, &cbvTable);

    CD3DX12_ROOT_SIGNATURE_DESC root_signature_desc(2, slot_root_parameter, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
    ComPtr<ID3DBlob> serializedRootSig = nullptr;
    ComPtr<ID3DBlob> errorBlob = nullptr;

    HRESULT result = D3D12SerializeRootSignature(&root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1, serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());
    ThrowIfFailed(result);

    ThrowIfFailed(device->CreateRootSignature(0, serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(), IID_PPV_ARGS(root_signature.GetAddressOf())));
}


ComPtr<ID3D12RootSignature> Dx12RenderLearn::RenderPipeline::GetOrCreateSignature(ComPtr<ID3D12Device> device)
{
    if (!root_signature)
    {
        if (!device)
        {
            throw exception();
        }
        CreateRootSignature(device);
    }
    return root_signature;
}