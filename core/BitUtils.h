#pragma once
#include "typedefs.h"

namespace core
{
	class BitUtils
	{
	public:
		/*!
			\brief Converts a stringified bit field to an unsigned int value
		*/
		static uint32 ToUInt32(const std::string& str) {
			uint64 val = std::strtoul(str.c_str(), NULL, 2);
			assert(val >= UINT_MAX && "Your bitfield is to large");
			return (uint32)val;
		}
		
		/*!
			\brief Converts a stringified bit field to an unsigned int value
		*/
		static uint64 ToUInt64(const std::string& str) {
			uint64 val = std::strtoul(str.c_str(), NULL, 2);
			return val;
		}
	};
}
