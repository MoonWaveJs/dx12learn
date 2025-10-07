#pragma once
#include "Material.h"
#include "Mesh.h"

namespace Dx12RenderLearn
{
    class Model
    {
    public:
        std::shared_ptr<Mesh> mesh;
        vector<std::shared_ptr<Material>> materials; // 0 for error mat
        Model(string& path);
    };
}