#include "Model.h"
namespace Dx12RenderLearn 
{
    Dx12RenderLearn::Model::Model(string& path)
    {
        mesh = std::make_shared<Mesh>(path);
		materials.push_back(std::make_shared<Material>());
    }
}