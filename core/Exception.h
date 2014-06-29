#pragma once
#include "typedefs.h"
#include <stdarg.h>

#define THROW_EXCEPTION(E, Message, ...) throw E(__FUNCTION__, __LINE__, __FILE__, Message, __VA_ARGS__)

namespace core
{
	//
	// Base class for all exception in the game engine.
	class Exception
	{
	public:
		Exception(const char* function, const long line, const char* file);
		Exception(const char* function, const long line, const char* file, const char* message, ...);
		virtual ~Exception();

		//
		// @return The exception message
		inline const std::string& GetMessage() const {
			return mMessage;
		}

		//
		// @return The function where the exception originated
		inline const std::string& GetFunction() const {
			return mFunction;
		}

		//
		// @return The file where the exception originated
		inline const std::string& GetFile() const {
			return mFile;
		}

		//
		// @return The line where the exception originated
		inline const uint64 GetLine() const {
			return mLine;
		}

	protected:
		void BuildMessage(const char* format, va_list argp);

	private:
		std::string mFunction;
		std::string mFile;
		std::string mMessage;
		uint64 mLine;
	};
}
