#include "IEntity.h"
#include "../Helper/d3dx12.h"
#include "../Helper/PlatformHelpers.h"
using namespace Microsoft::WRL;


void Dx12RenderLearn::IEntity::InitTransformBuffer()
{

}

void Dx12RenderLearn::IEntity::SetScale(float x, float y, float z)
{
	mTransform *= XMMatrixScaling(x, y, z);
}

void Dx12RenderLearn::IEntity::SetPosition(float x, float y, float z)
{
	mTransform *= XMMatrixTranslation(x, y, z);
}
