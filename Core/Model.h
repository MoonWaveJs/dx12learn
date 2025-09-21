#pragma once
#include "Material.h"
#include "Mesh.h"

#define PGETMODELMESH(model,i) (model->meshs->at(i))
#define PGETMODELMATRIAL(model,i) (model->materials->at(i))
namespace Dx12RenderLearn
{
    class Model
    {
    public:
        std::shared_ptr<vector<std::shared_ptr<Mesh>>> meshs;
        std::shared_ptr<vector<std::shared_ptr<Material>>> materials; // 0 for error mat
        UINT SectionsNum{ 1 };
        Model();
    };
}