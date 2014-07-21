#pragma once
#include "../typedefs.h"
#include "../math/Matrix4x4.h"
#include "Frustum.h"

namespace core
{
	/*!
		\brief Class representing a projector.

		A projector has a frustum, which is useful for frustum-culling. It also contains a perspective- and view matrix, which is useful when rendering things
		from the projector's point of view.
	*/
	class Projector
	{
	public:
		Projector();
		Projector(const Projector& rhs);
		~Projector();

		void SetPerspective(float32 near, float32 far, float32 fov, float32 ratio);
		void SetOrtho2D(float32 left, float32 right, float32 bottom, float32 top);
		void LookAt(const Vector3& eye, const Vector3& direction, const Vector3& up);

		/*!
			\brief Retrieves this projectors frustum
		*/
		inline const Frustum* GetFrustum() const {
			return &mFrustum;
		}

		/*!
			\brief Retrieves the view matrix
		*/
		inline const Matrix4x4& GetViewMatrix() const {
			return mViewMatrix;
		}

		/*!
			\brief Retrieves the projection matrix
		*/
		inline const Matrix4x4& GetProjectionMatrix() const {
			return mProjectionMatrix;
		}
		
		/*!
			\brief Retrieves the near clip distance
		*/
		inline float32 GetNearClipDistance() const {
			return mNearClipDistance;
		}

		/*!
			\brief Retrieves the far clip distance
		*/
		inline float32 GetFarClipDistance() const {
			return mFarClipDistance;
		}

	private:
		Frustum mFrustum;
		Matrix4x4 mViewMatrix;
		Matrix4x4 mProjectionMatrix;

		float32 mNearClipDistance;
		float32 mFarClipDistance;
	};

}
