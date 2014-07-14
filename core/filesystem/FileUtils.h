#pragma once
#include "../typedefs.h"
#include <random>

namespace core
{
	static const std::string TEMP_DIR = "/tmp";
	static const std::string TEMP_DIR_RANDOM_CHARS = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

	class FileUtils
	{
	public:
		static std::string GetTempPath(const std::string& suffix) {
			return GetTempPath(std::string(), suffix);
		}

		static std::string GetTempPath(const std::string& prefix, const std::string& suffix) {
			static std::default_random_engine generator;
			std::uniform_int_distribution<uint32> distribution(0, TEMP_DIR_RANDOM_CHARS.length());
			std::string tempDir = TEMP_DIR + "/" + prefix;
			for (size_t i = 0; i < 15; ++i) {
				uint32 idx = distribution(generator);
				tempDir += TEMP_DIR_RANDOM_CHARS[idx];
			}
			tempDir += suffix;
			return tempDir;
		}
	};
}
