#include "../../Memory.h"
#include "Sphere.h"
#include "../RenderContext.h"
#include "../VertexBufferUtils.h"
using namespace core;

Sphere::Sphere(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, float32 radius, uint32 resolution)
: mVertexBuffer(vertexBuffer), mIndexBuffer(indexBuffer), mRadius(radius), mResolution(resolution)
{

}

Sphere::~Sphere()
{
	if (mIndexBuffer != nullptr) {
		delete mIndexBuffer;
		mIndexBuffer = nullptr;
	}

	if (mVertexBuffer != nullptr) {
		delete mVertexBuffer;
		mVertexBuffer = nullptr;
	}
}

Sphere* Sphere::Create(float32 radius, uint32 resolution, BufferUsage::Enum usage)
{
	return new Sphere(VertexBufferUtils::CreateSphere(radius, resolution, 0, 0, 0, usage), nullptr, radius, resolution);
}
