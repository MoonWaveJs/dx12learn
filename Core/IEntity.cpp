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

void Dx12RenderLearn::IEntity::SetRotation(float x, float y, float z, float w)
{
	// XMMATRIX R = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, z);
	// XMMATRIX P = XMMatrixRotationRollPitchYaw(x, 0.0f, 0.0f);
	// XMMATRIX Y = XMMatrixRotationRollPitchYaw(0.0f, y, 0.0f);
	//
	XMVECTOR quat = XMVectorSet(x, y, z, w);
	mTransform *= XMMatrixRotationQuaternion(quat);
}
