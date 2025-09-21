#include "Model.h"
namespace Dx12RenderLearn 
{
    Model::Model()
    {
        meshs = std::make_shared<vector<std::shared_ptr<Mesh>>>();
        materials = std::make_shared<vector<std::shared_ptr<Material>>>();

		meshs->push_back(std::make_shared<Mesh>());
		materials->push_back(std::make_shared<Material>());
		SectionsNum = 1;
    }
}