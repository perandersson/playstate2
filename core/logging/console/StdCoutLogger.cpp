#include "../../Memory.h"
#include "StdCoutLogger.h"
#include <iostream>
using namespace core;

StdCoutLog::StdCoutLog()
{
}

StdCoutLog::~StdCoutLog()
{
}

void StdCoutLog::Error(const char* msg)
{
	std::cout << "[ERROR] " << msg << std::endl;
}

void StdCoutLog::Info(const char* msg)
{
	std::cout << "[INFO] " << msg << std::endl;
}

void StdCoutLog::Debug(const char* msg)
{
	std::cout << "[DEBUG] " << msg << std::endl;
}

void StdCoutLog::Warn(const char* msg)
{
	std::cout << "[WARN] " << msg << std::endl;
}
