#pragma once
#include <memory>

#include "IEntity.h"
#include "Model.h"
namespace Dx12RenderLearn
{
    class StaticMeshEntity :public Dx12RenderLearn::IEntity
    {
    public:
        std::shared_ptr<Model> model;
        StaticMeshEntity();
    };
}