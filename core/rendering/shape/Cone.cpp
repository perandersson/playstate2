#include "../../Memory.h"
#include "Cone.h"
#include "../RenderContext.h"
#include "../VertexBufferUtils.h"
using namespace core;

Cone::Cone(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, const Vector3& apex, const Vector3& direction, float32 height, float32 radius, uint32 resolution)
: mVertexBuffer(vertexBuffer), mIndexBuffer(indexBuffer), mApex(apex), mDirection(direction), mHeight(height), mRadius(radius), mResolution(resolution)
{

}

Cone::~Cone()
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

void Cone::Update(const Vector3& apex, const Vector3& direction, float32 height, float32 radius, uint32 resolution)
{
	assert_not_null(mVertexBuffer);
	assert(mVertexBuffer->GetBufferUsage() == BufferUsage::DYNAMIC && "You cannot update a static shape");

	std::vector<PositionVertexType> vertices;
	GenerateVertices(apex, -direction, height, radius, resolution, vertices);
	mVertexBuffer->Update(&vertices[0], vertices.size());

	mApex = apex;
	mDirection = direction;
	mHeight = height;
	mRadius = radius;
	mResolution = resolution;
}

Cone* Cone::Create(const Vector3& apex, const Vector3& direction, float32 height, float32 radius, uint32 resolution, BufferUsage::Enum usage)
{
	std::vector<PositionVertexType> vertices;
	GenerateVertices(apex, -direction, height, radius, resolution, vertices);
	VertexBuffer* vertexBuffer = RenderContext::CreateBuffer(&vertices[0], PrimitiveType::TRIANGLE, vertices.size(), usage);
	return new Cone(vertexBuffer, nullptr, apex, direction, height, radius, resolution);
}

void Cone::GenerateVertices(const Vector3& apex, const Vector3& direction, float32 height, float32 radius, uint32 resolution, std::vector<PositionVertexType>& _out_Vertices)
{
	const Vector3 c = apex + (-direction * height);
	const Vector3 e0 = direction.Perp();
	const Vector3 e1 = e0.CrossProduct(direction);
	const float64 angleIncr = 360.0 / resolution * M_PI_DIV180;

	std::vector<Vector3> circlePoints;
	for (uint32 i = 0; i < resolution; ++i) {
		const float64 rad = angleIncr * i;
		Vector3 p = c + (((e0 * (float32)cos(rad)) + (e1 * (float32)sin(rad))) * radius);
		circlePoints.push_back(p);
	}

	//
	// Bottom Circle
	//

	for (uint32 i = 1; i < resolution; ++i) {
		_out_Vertices.push_back(c);
		_out_Vertices.push_back({ circlePoints[i - 1] });
		_out_Vertices.push_back({ circlePoints[i] });
	}

	_out_Vertices.push_back(c);
	_out_Vertices.push_back({ circlePoints[resolution - 1] });
	_out_Vertices.push_back({ circlePoints[0] });

	//
	// Top cylinder
	//

	for (uint32 i = 1; i < resolution; ++i) {
		_out_Vertices.push_back(apex);
		_out_Vertices.push_back({ circlePoints[i - 1] });
		_out_Vertices.push_back({ circlePoints[i] });
	}

	_out_Vertices.push_back(apex);
	_out_Vertices.push_back({ circlePoints[resolution - 1] });
	_out_Vertices.push_back({ circlePoints[0] });
}
