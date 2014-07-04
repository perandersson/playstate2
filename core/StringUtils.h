#pragma once
#include "typedefs.h"
#include <vector>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <stdarg.h>

namespace core
{
	class StringUtils
	{
	public:
		static std::string ToString(uint32 i) {
			char tmp[32];
			_itoa(i, tmp, 10);
			return tmp;
		}

		static float32 ToFloat32(const std::string& str) {
			float32 val = 0.0f;
			sscanf(str.c_str(), "%f", &val);
			return val;
		}

		static int32 ToInt32(const std::string& str) {
			int64 val = std::strtol(str.c_str(), NULL, 10);
			return (int32)val;
		}

		static uint32 ToUInt32(const std::string& str) {
			uint64 val = std::strtoul(str.c_str(), NULL, 10);
			return (uint32)val;
		}

		static std::vector<std::string> Split(const std::string& str, const char delim) {
			std::vector<std::string> elems;
			return Split(str, delim, elems);
		}

		static std::string RemoveWhitespaces(const std::string& str) {
			return Replace(Replace(Replace(str, '\r', std::string("")), '\t', std::string()), '\n', std::string());
		}

		static std::string Replace(const std::string& value, const char replace, const std::string& newval) {
			std::string result;
			for (size_t i = 0; i < value.size(); ++i) {
				if (value[i] == replace) {
					result += newval;
				}
				else {
					result += value[i];
				}
			}
			return result;
		}

		static std::string ToLowerCase(const std::string& otherString) {
			std::string data = otherString;
			std::transform(data.begin(), data.end(), data.begin(), ::tolower);
			return data;
		}
		
		static bool ToBool(const std::string& str) {
			return ToLowerCase(str) == std::string("true");
		}

		static std::string ToUpperCase(const std::string& otherString) {
			std::string data = otherString;
			std::transform(data.begin(), data.end(), data.begin(), ::toupper);
			return data;
		}

		static std::string ToLowerCase(const char* otherString) {
			std::string data(otherString);
			std::transform(data.begin(), data.end(), data.begin(), ::tolower);
			return data;
		}

		static std::string Evaluate(const char* fml, ...) {
			va_list arglist;
			va_start(arglist, fml);
			char tmp[5096];
			vsprintf_s(tmp, 5096, fml, arglist);
			va_end(arglist);
			return std::string(tmp);
		}

	private:
		static std::vector<std::string>& Split(const std::string& str, const char delim, std::vector<std::string>& elems) {
			std::stringstream ss(str);
			std::string item;
			while (std::getline(ss, item, delim)) {
				elems.push_back(item);
			}
			return elems;
		}
	};
}