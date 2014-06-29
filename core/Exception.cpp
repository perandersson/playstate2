#include "Memory.h"
#include "Exception.h"
using namespace core;

Exception::Exception(const char* function, const long line, const char* file)
: mFunction(function), mLine(line), mFile(file)
{
	std::string::size_type index = mFile.find(std::string("\\core\\"));
	if (index == std::string::npos) {
		index = mFile.find(std::string("/core/"));
	}

	mFile = mFile.substr(index);
}

Exception::Exception(const char* function, const long line, const char* file, const char* message, ...)
: Exception(function, line, file)
{
	va_list arglist;
	va_start(arglist, message);
	BuildMessage(message, arglist);
	va_end(arglist);
}

Exception::~Exception()
{
}

void Exception::BuildMessage(const char* format, va_list argp)
{
	char tmp[5096];
	vsprintf_s(tmp, 5096, format, argp);
	mMessage = tmp;
}
