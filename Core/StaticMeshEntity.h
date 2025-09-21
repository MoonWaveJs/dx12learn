#pragma once
#include <memory>

#include "IEntity.h"
#include "Model.h"
namespace Dx12RenderLearn
{
	using namespace std;
    class StaticMeshEntity :public Dx12RenderLearn::IEntity
    {
    public:
        std::shared_ptr<Model> Model;
        StaticMeshEntity();
    };
}