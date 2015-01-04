#pragma once
#include "IKernel.h"

namespace core
{
	class IEventDrivenKernel : public IKernel
	{
	public:
		/*!
			\brief Initialize the kernel. 
		*/
		virtual bool Initialize() = 0;

		/*!
			\brief Releases this kernels internal resources
		*/
		virtual void Release() = 0;

		/*!
			\brief Process the internal kernel events.

			If the kernel has decided to stop processing it's internal events it usually means that the game window has been closed or
			if the game has been closed by an external party (i.e. the operating system)

			\return TRUE if the kernel proccessed the events correctly; FALSE otherwise. 
		*/
		virtual bool ProcessEvents() = 0;
	};
	

	/*!
		\brief Create a kernel instance. 
		
		\param params
					Extra parameters
		\return A kernel instance
	*/
	extern IEventDrivenKernel* CreateKernel(void* params);

	/*!
		\brief Destroy the supplied kernel
		
		\param kernel
					The kernel we want to destroy
	*/
	extern void DestroyKernel(IEventDrivenKernel* kernel);
}