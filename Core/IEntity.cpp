#include "IEntity.h"
#include "../Helper/d3dx12.h"
#include "../Helper/PlatformHelpers.h"
using namespace Microsoft::WRL;


void Dx12RenderLearn::IEntity::Awake()
{
	isDirty = true;
	
}

void Dx12RenderLearn::IEntity::Update()
{
	if (isDirty)
	{

	}
}

void Dx12RenderLearn::IEntity::InitTransformBuffer()
{

}
