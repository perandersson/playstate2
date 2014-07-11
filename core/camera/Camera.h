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

		void SetPerspective(float32 near, float32 far, float32 fov, float32 ratio);
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

		inline float32 GetNearClipDistance() const {
			return mNearPlane;
		}

		/*!
			\brief
		*/
		inline float32 GetFarClipDistance() const {
			return mFarPlane;
		}

		/*!
		
		*/
		inline const Vector3& GetLookingAtDirection() const {
			return mLookingAtDirection;
		}

	public:
		/*!
			\brief Generate a orthographic projection matrix

			\param left
			\param right
			\param bottom
			\param top

			\return
		*/
		static Matrix4x4 GetOrtho2D(float32 left, float32 right, float32 bottom, float32 top);

		/*!
			\brief Generates a perspective projection matrix

			\param near
			\param far
			\param fov
			\param ratio

			\return
		*/
		static Matrix4x4 GetPerspective(float32 near, float32 far, float32 fov, float32 ratio);

		/*!
			\brief Generates a look-at view matrix

			\param eye
			\param center
			\param up
			\return
		*/
		static Matrix4x4 GetLookAt(const Vector3& eye, const Vector3& center, const Vector3& up);

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
