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

		/*!
			\brief Retrieves the camera's view frustum

			\return The camera's view frustum
		*/
		inline const Frustum* GetFrustum() const {
			return &mFrustum;
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

		/*!
			\brief Retrieves the camera's position
		*/
		inline const Vector3& GetPosition() const {
			return mPosition;
		}

		/*!
			\brief Retrieves the up vector for the camera
		*/
		inline const Vector3& GetUp() const {
			return mUp;
		}

		/*!
			\brief Retrieves the position where this camera is focusing on
		*/
		inline const Vector3& GetCenter() const {
			return mCenter;
		}
		
		/*!
			\brief Retrieves the near clip distance
		*/
		inline float32 GetNearClipDistance() const {
			return mNearPlane;
		}

		/*!
			\brief Retrieves the far clip distance
		*/
		inline float32 GetFarClipDistance() const {
			return mFarPlane;
		}

		/*!
			\brief Retrieves the view direction
		*/
		inline const Vector3& GetViewDirection() const {
			return mViewDirection;
		}

	private:
		Frustum mFrustum;

		Matrix4x4 mViewMatrix;
		Matrix4x4 mProjectionMatrix;

		Vector3 mPosition;
		Vector3 mCenter;
		Vector3 mUp;

		float32 mNearPlane;
		float32 mFarPlane; 

		Vector3 mViewDirection;
	};
}
