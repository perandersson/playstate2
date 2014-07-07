#include "VertexBufferUtils.h"
#include "RenderContext.h"
#include "../math/MathUtils.h"
using namespace core;

VertexBuffer* VertexBufferUtils::CreateSphere(float32 R, uint32 resolution, float32 H, float32 K, float32 Z, BufferUsage::Enum usage) {
	const uint32 SPACE = resolution;
	const uint32 VERTEX_COUNT = (180 / SPACE) * (360 / SPACE) * 4;

	std::vector<PositionTextureVertexType> vertices(VERTEX_COUNT);
	uint32 n = 0;
	for (float32 b = 0; b <= 180 - SPACE; b += SPACE) {
		for (float32 a = 0; a <= 360 - SPACE; a += SPACE){
			vertices[n].pos.x = R * sinf((a) / 180.0f * M_PI_F) * sinf((b) / 180.0f * M_PI_F) - H;
			vertices[n].pos.y = R * cosf((a) / 180.0f * M_PI_F) * sinf((b) / 180.0f * M_PI_F) + K;
			vertices[n].pos.z = R * cosf((b) / 180.0f * M_PI_F) - Z;
			vertices[n].texCoord.t = (2 * b) / 360.0f;
			vertices[n].texCoord.s = (a) / 360.0f;
			n++;

			vertices[n].pos.x = R * sinf((a) / 180.0f * M_PI_F) * sinf((b + SPACE) / 180.0f * M_PI_F) - H;
			vertices[n].pos.y = R * cosf((a) / 180.0f * M_PI_F) * sinf((b + SPACE) / 180.0f * M_PI_F) + K;
			vertices[n].pos.z = R * cosf((b + SPACE) / 180.0f * M_PI_F) - Z;
			vertices[n].texCoord.t = (2 * (b + SPACE)) / 360.0f;
			vertices[n].texCoord.s = (a) / 360.0f;
			n++;

			vertices[n].pos.x = R * sinf((a + SPACE) / 180.0f * M_PI_F) * sinf((b) / 180.0f * M_PI_F) - H;
			vertices[n].pos.y = R * cosf((a + SPACE) / 180.0f * M_PI_F) * sinf((b) / 180.0f * M_PI_F) + K;
			vertices[n].pos.z = R * cosf((b) / 180.0f * M_PI_F) - Z;
			vertices[n].texCoord.t = (2 * b) / 360.0f;
			vertices[n].texCoord.s = (a + SPACE) / 360.0f;
			n++;

			vertices[n].pos.x = R * sinf((a + SPACE) / 180.0f * M_PI_F) * sinf((b + SPACE) / 180.0f * M_PI_F) - H;
			vertices[n].pos.y = R * cosf((a + SPACE) / 180.0f * M_PI_F) * sinf((b + SPACE) / 180.0f * M_PI_F) + K;
			vertices[n].pos.z = R * cosf((b + SPACE) / 180.0f * M_PI_F) - Z;
			vertices[n].texCoord.t = (2 * (b + SPACE)) / 360.0f;
			vertices[n].texCoord.s = (a + SPACE) / 360.0f;
			n++;
		}
	}

	return RenderContext::CreateBuffer(&vertices[0], PrimitiveType::TRIANGLE_STRIP, vertices.size(), usage);
}

VertexBuffer* VertexBufferUtils::CreateCone(const Vector3& d, const Vector3& a, float32 H, float32 Rd, uint32 n)
{
	const Vector3 c = a + (-d * H);
	const Vector3 e0 = d.Perp();
	const Vector3 e1 = e0.CrossProduct(d);
	const float64 angleIncr = 360.0 / n * M_PI_DIV180;

	std::vector<Vector3> circlePoints;
	for (uint32 i = 0; i < n; ++i) {
		const float64 rad = angleIncr * i;
		Vector3 p = c + (((e0 * (float32)cos(rad)) + (e1 * (float32)sin(rad))) * Rd);
		circlePoints.push_back(p);
	}

	std::vector<PositionVertexType> vertices;

	//
	// Bottom Circle
	//

	for (uint32 i = 1; i < n; ++i) {
		vertices.push_back(c);
		vertices.push_back({ circlePoints[i - 1] });
		vertices.push_back({ circlePoints[i] });
	}

	vertices.push_back(c);
	vertices.push_back({ circlePoints[n - 1] });
	vertices.push_back({ circlePoints[0] });

	//
	// Top cylinder
	//

	for (uint32 i = 1; i < n; ++i) {
		vertices.push_back(a);
		vertices.push_back({ circlePoints[i - 1] });
		vertices.push_back({ circlePoints[i] });
	}

	vertices.push_back(a);
	vertices.push_back({ circlePoints[n - 1] });
	vertices.push_back({ circlePoints[0] });

	return RenderContext::CreateBuffer(&vertices[0], PrimitiveType::TRIANGLE, vertices.size(), BufferUsage::STATIC);
}

VertexBuffer* VertexBufferUtils::CreateSpotLight(const Vector3& position, const Vector3& direction, float32 cutoff)
{
	const Vector3 d = -direction;
	const Vector3 a = position;
	const uint32 n = 20U;
	const float32 H = 100.0f;
	
	return CreateCone(d, a, H, cutoff, n);
}

VertexBuffer* VertexBufferUtils::CreateFullscreenQuad() {
	PositionTextureVertexType elements[6];
	elements[0].pos = Vector3(-1.0f, 1.0f, 0.0f);
	elements[0].texCoord = Vector2(0.0f, 1.0f);
	elements[1].pos = Vector3(1.0f, 1.0f, 0.0f);
	elements[1].texCoord = Vector2(1.0f, 1.0f);
	elements[2].pos = Vector3(-1.0f, -1.0f, 0.0f);
	elements[2].texCoord = Vector2(0.0f, 0.0f);
	elements[3].pos = Vector3(-1.0f, -1.0f, 0.0f);
	elements[3].texCoord = Vector2(0.0f, 0.0f);
	elements[4].pos = Vector3(1.0f, 1.0f, 0.0f);
	elements[4].texCoord = Vector2(1.0f, 1.0f);
	elements[5].pos = Vector3(1.0f, -1.0f, 0.0f);
	elements[5].texCoord = Vector2(1.0f, 0.0f);
	return RenderContext::CreateBuffer(&elements[0], PrimitiveType::TRIANGLE, 6, BufferUsage::STATIC);
}
