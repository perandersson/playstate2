#pragma once
#include "../math/Vector3.h"

namespace core
{
	class ISoundReceiver
	{
	public:
		virtual ~ISoundReceiver() {}

		//
		//
		virtual void SetPosition(const Vector3& position) = 0;

		//
		//
		virtual void LookAt(const Vector3& direction, const Vector3& up) = 0;
	};

}
