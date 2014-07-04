#pragma once
#include "VertexBuffer.h"
#include "VertexTypes.h"
#include "RenderContext.h"
#include "../math/MathUtils.h"

namespace core
{
	class VertexBufferUtils
	{
	public:
		/*!
			\brief Create a spere
		*/
		static VertexBuffer* CreateSphere(float32 R, float32 H, float32 K, float32 Z, BufferUsage::Enum usage) {
			const uint32 SPACE = 10;
			const uint32 VERTEX_COUNT = (90 / SPACE) * (360 / SPACE) * 4;

			std::vector<PositionTextureVertexType> vertices(VERTEX_COUNT);
			uint32 n = 0;
			for (float32 b = 0; b <= 90 - SPACE; b += SPACE) {
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

			return RenderContext::CreateBuffer(&vertices[0], vertices.size(), usage);
		}

		/*!
			\brief Create a fullscreen [-1, 1] quad 
		*/
		static VertexBuffer* CreateFullscreenQuad() {
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
			return RenderContext::CreateBuffer(&elements[0], 6, BufferUsage::STATIC);
		}
	};
}
