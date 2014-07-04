#pragma once
#include "LightSource.h"
#include "../../rendering/Texture.h"

namespace core
{
	class SpotLight : public LightSource
	{
		DECLARE_SCRIPT_OBJECT(SpotLight);

	public:
		SpotLight();
		virtual ~SpotLight();
		
		/*!
			\brief Set the color for this spotlight

			\param color
		*/
		inline void SetColor(const Color& color) {
			mColor = color;
		}

		/*!
			\brief Retrieves the color for this spotlight

			\return The color
		*/
		inline const Color& GetColor() const {
			return mColor;
		}

		/*!
			\brief Set this spotlight's cutoff angle (in degrees)

			\param cutoff
						The angle in degrees
		*/
		inline void SetCutoff(float32 cutoff) {
			mCutoff = cutoff;
		}

		/*!
			\brief Retrieves this spotlight's cutoff angle

			\return The cutoff angle in degrees
		*/
		inline float32 GetCutoff() const {
			return mCutoff;
		}

		/*!
			\brief Set the direction this spotlight is looking towards

			\param direction
					A direction (normalized) to where this spotlight is looking towards
		*/
		inline void SetDirection(const Vector3& direction) {
			mDirection = direction;
		}

		/*!
			\brief Retrieves the direction where this spotlight is lookat towards
		*/
		inline const Vector3& GetDirection() const {
			return mDirection;
		}

		/*!
			\brief Set the texture this spotlight is projecting onto the screen

			\param texture
		*/
		inline void SetTexture(Resource<Texture> texture) {
			mTexture = texture;
		}

		/*!
			\brief Retrieves the texture this spotlight is projecting
		*/
		inline Resource<Texture> GetTexture() const {
			return mTexture;
		}
		
		virtual void CollectLightBlocks(const FindQuery& query, LightSourceResultSet* _out_resultSet) const;

	private:
		Color mColor;
		float32 mCutoff;
		Vector3 mDirection;
		Resource<Texture> mTexture;
	};
}
