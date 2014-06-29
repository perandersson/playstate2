#pragma once

namespace core
{
	class ILogger
	{
	public:
		virtual ~ILogger() {}

	public:
		virtual void Error(const char* msg) = 0;
		virtual void Info(const char* msg) = 0;
		virtual void Debug(const char* msg) = 0;
		virtual void Warn(const char* msg) = 0;
	};
}
