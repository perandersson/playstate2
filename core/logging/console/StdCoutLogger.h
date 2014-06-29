#pragma once
#include "../ILogger.h"

namespace core
{
	class StdCoutLog : public ILogger
	{
	public:
		StdCoutLog();
		virtual ~StdCoutLog();

		/*!
			\see ILogger::Error
		*/
		virtual void Error(const char* msg);
		
		/*!
			\see ILogger::Info
		*/
		virtual void Info(const char* msg);
		
		/*!
			\see ILogger::Debug
		*/
		virtual void Debug(const char* msg);
		
		/*!
			\see ILogger::Warn
		*/
		virtual void Warn(const char* msg);
	};
}
