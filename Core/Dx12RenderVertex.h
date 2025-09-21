#pragma once
#include <d3d12.h>
#include  <DirectXMath.h>
#include <DirectXPackedVector.h>
namespace Dx12RenderLearn
{
    using namespace  DirectX;
    struct Dx12RenderVertex
    {
        XMFLOAT3 Position;
        XMFLOAT3 Normal;
        XMFLOAT4 Color;
        XMFLOAT4 TextureCoord0;

        static D3D12_INPUT_LAYOUT_DESC GetInputLayoutDesc()
        {
            D3D12_INPUT_LAYOUT_DESC InputLayoutDesc;
            InputLayoutDesc.NumElements = 4;
            UINT Vector3Size = sizeof(XMFLOAT3);
            UINT Vector4Size = sizeof(XMFLOAT4);
            static D3D12_INPUT_ELEMENT_DESC Element_desc[] =         {
                {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
                {"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,Vector3Size,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
                {"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,Vector3Size*2,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
                {"TEX",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,Vector3Size*2+Vector4Size,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0}
            };
            InputLayoutDesc.pInputElementDescs = Element_desc;
            return InputLayoutDesc;
        }

        Dx12RenderVertex(XMFLOAT3 Position,XMFLOAT3 Normal,XMFLOAT4 Color, XMFLOAT4 TextureCoord0)
        {
            this->Position = Position;
            this->Color = Color;
            this->Normal = Normal;
            this->TextureCoord0 = TextureCoord0;
        }

        Dx12RenderVertex(XMFLOAT3 Position,XMFLOAT4 Color): Dx12RenderVertex(Position, XMFLOAT3(0.0f, 0.0f, 0.0f), Color, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f))
        {}
    };
}
