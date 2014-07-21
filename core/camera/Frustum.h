#pragma once
#include "../math/Vector3.h"
#include "../math/Plane.h"
#include "../collision/AABB.h"
#include "FrustumPlanes.h"

namespace core
{
	// http://fgiesen.wordpress.com/2010/10/17/view-frustum-culling/
	// http://blog.makingartstudios.com/?p=155
	// http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-implementation/
	//
	class Frustum
	{
	public:
		Frustum();
		Frustum(const Frustum& frustum);
		Frustum(const Vector3& eye, const Vector3& center, const Vector3& up,
			float32 nearPlane, float32 farPlane, float32 fov, float32 ratio);
		~Frustum();

		/*!
			\brief Generates the frustums corners based on which direction the frustum is looking at

			\param eye
			\param direction
			\param up
		*/
		void LookAt(const Vector3& eye, const Vector3& direction, const Vector3& up);

		//
		// Sets the perspective for this frustum
		//
		// @param nearPlane
		// @param farPlane
		// @param fov
		// @param ratio
		void SetPerspective(float32 nearPlane, float32 farPlane, float32 fov, float32 ratio);

		/*!
			\brief Check to see if the supplied axis-align bounding box collides with this frustum

			\param box
			\return
		*/
		CollisionResult::Enum IsColliding(const AABB& box) const;

	private:
		Vector3 GetPositiveVertex(const AABB& boundingBox, const Vector3& normal) const;
		Vector3 GetNegativeVertex(const AABB& boundingBox, const Vector3& normal) const;
		
	private:
		Plane mPlanes[FrustumPlanes::NUM_PLANES];

		float32 mNearPlane;
		float32 mFarPlane;
		float32 mFov;
		float32 mRatio;

		float32 mTang;
		float32 mNH;
		float32 mNW;
		float32 mFH;
		float32 mFW;
	};
}
