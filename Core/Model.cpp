#include "Model.h"
namespace Dx12RenderLearn 
{
    Model::Model()
    {
        mesh = std::make_shared<Mesh>();
		materials.push_back(std::make_shared<Material>());
    }
    UINT Model::GetVertexNum()
    {
        int n = 0;
        for (auto m : mesh->GetVertexNums())
        {
            n += m;
        }
        return n;
    }
    UINT Model::GetIndicesNum()
    {
        int n = 0;
        for (auto m : mesh->GetIndicesNums())
        {
            n += m;
        }
        return n;
    }
}