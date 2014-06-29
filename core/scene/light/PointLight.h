#pragma once
#include "LightSource.h"
#include "../../math/Color.h"

namespace core
{
	/*!
		\brief 
	*/
	class PointLight : public LightSource
	{
		DECLARE_SCRIPT_OBJECT(PointLight);

	public:
		PointLight();
		virtual ~PointLight();

		void SetOffset(const Vector3& offset);
		void SetColor(const Color& color);
		void SetRadius(float32 radius);
		void SetConstantAttenuation(float32 constantAttenuation);
		void SetLinearAttenuation(float32 linearAttenuation);
		void SetQuadricAttenuation(float32 quadricAttenuation);
		
		inline const Vector3& GetOffset() const {
			return mOffset;
		}
		
		inline const Color& GetColor() {
			return mColor;
		}

		inline float32 GetRadius() const {
			return mRadius;
		}
		
		inline float32 GetConstantAttenuation() const {
			return mConstantAttenuation;
		}

		inline float32 GetLinearAttenuation() const {
			return mLinearAttenuation;
		}

		inline float32 GetQuadricAttenuation() const {
			return mQuadricAttenuation;
		}

	public:
		virtual void OnAddedToSceneGroup();
		virtual void CollectLightBlocks(const FindQuery& query, LightSourceResultSet* _out_resultSet) const;

	private:
		Vector3 mOffset;
		Color mColor;
		float32 mRadius;
		float32 mConstantAttenuation;
		float32 mLinearAttenuation;
		float32 mQuadricAttenuation;
	};
}
