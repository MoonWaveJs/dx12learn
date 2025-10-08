#include "StaticMeshEntity.h"


Dx12RenderLearn::StaticMeshEntity::StaticMeshEntity(std::string modelPath)
{
	model = std::make_shared<Model>(modelPath);
}
