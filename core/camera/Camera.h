#pragma once
#include "Frustum.h"
#include "../math/Matrix4x4.h"
#include "../script/ScriptObject.h"

namespace core
{
	//
	//
	class Camera : public ScriptObject
	{
		DECLARE_SCRIPT_OBJECT(Camera);

	public:
		Camera();
		Camera(const Camera& camera);
		virtual ~Camera();

		void SetPerspective(float32 nearPlane, float32 farPlane, float32 fov, float32 ratio);
		void SetOrtho2D(float32 left, float32 right, float32 bottom, float32 top);

		void Move(const Vector3& direction);
		void LookAt(const Vector3& eye, const Vector3& center, const Vector3& up);

		//
		// @return This camera's view frustum
		inline const Frustum& GetViewFrustum() const {
			return mViewFrustum;
		}

		//
		// @return This camera's view matrix
		inline const Matrix4x4& GetViewMatrix() const {
			return mViewMatrix;
		}

		//
		// @return This camera's projection matrix
		inline const Matrix4x4& GetProjectionMatrix() const {
			return mProjectionMatrix;
		}

		//
		// @return This camera's position
		inline const Vector3& GetPosition() const {
			return mPosition;
		}

		//
		// @return This camera's up vector (which way is up)
		inline const Vector3& GetUp() const {
			return mUp;
		}

		//
		// @return The position where this camera is focusing on
		inline const Vector3& GetCenter() const {
			return mCenter;
		}

		inline float32 GetNearPlane() const {
			return mNearPlane;
		}

		/*!
			\brief
		*/
		inline float32 GetFarPlane() const {
			return mFarPlane;
		}

		/*!
		
		*/
		inline const Vector3& GetLookingAtDirection() const {
			return mLookingAtDirection;
		}

	public:
		//
		// Generate a orthographic projection matrix and returns the result
		//
		// @param left
		// @param right
		// @param bottom
		// @param top
		// @return
		static Matrix4x4 GetOrtho2D(float32 left, float32 right, float32 bottom, float32 top);

	private:
		void CalculateLookAt(const Vector3& eye, const Vector3& center, const Vector3& up);
		
	private:
		Frustum mViewFrustum;
		Matrix4x4 mViewMatrix;
		Matrix4x4 mProjectionMatrix;

		Vector3 mPosition;
		Vector3 mCenter;
		Vector3 mUp;

		float32 mNearPlane;
		float32 mFarPlane; 

		Vector3 mLookingAtDirection;
	};
}
