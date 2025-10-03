#include "Mesh.h"

#include <DirectXColors.h>

std::vector<UINT> Dx12RenderLearn::Mesh::GetVertexNums()
{
    // load from meta file info
    return { 8 };
}

std::vector<UINT> Dx12RenderLearn::Mesh::GetIndicesNums()
{
    return { 36 };
}

void Dx12RenderLearn::Mesh::LoadVertexData(shared_ptr<vector<Dx12RenderVertex>>& pVertexBuffer)
{
    pVertexBuffer->push_back(Dx12RenderVertex(XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Red)));
    pVertexBuffer->push_back(Dx12RenderVertex(XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Black)));
    pVertexBuffer->push_back(Dx12RenderVertex(XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Red)));
    pVertexBuffer->push_back(Dx12RenderVertex(XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green)));
    pVertexBuffer->push_back(Dx12RenderVertex(XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Red)));
    pVertexBuffer->push_back(Dx12RenderVertex(XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Yellow)));
    pVertexBuffer->push_back(Dx12RenderVertex(XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Red)));
    pVertexBuffer->push_back(Dx12RenderVertex(XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Red)));
}

void Dx12RenderLearn::Mesh::LoadIndicesData(shared_ptr<vector<UINT>>& pIndexBuffer)
{
    UINT indices[] =
    {
        // 立方体前表面
        0, 1, 2,
        0, 2, 3,
        // 立方体后表面
        4, 6, 5,
        4, 7, 6,
        // 立方体左表面
        4, 5, 1,
        4, 1, 0,
        // 立方体右表面
        3, 2, 6,
        3, 6, 7,
        // 立方体上表面
        1, 5, 6,
        1, 6, 2,
        // 立方体下表面
        4, 0, 3,
        4, 3, 7
    };
    pIndexBuffer->insert(pIndexBuffer->end(), std::begin(indices), std::end(indices));
}
