#pragma once
#include <memory>
#include <string>
#include <cassert>

#ifdef __GNUC__
#include <ext/hash_map>
namespace std { using namespace __gnu_cxx; }
#else
#include <hash_map>
#endif

namespace core
{
	typedef char int8;
	typedef unsigned char uint8;

	typedef short int16;
	typedef unsigned short uint16;

	typedef int int32;
	typedef unsigned int uint32;

	typedef long long int64;
	typedef unsigned long long uint64;
	
	typedef unsigned char byte;

	typedef float float32;
	typedef double float64;

	typedef uint64 typemask;
}

#ifndef ENUM_STRING
#define ENUM_STRING(map, type, _enum) map##.insert(std::make_pair(#_enum##, type##::##_enum))
#endif

#ifndef OFFSET
#define OFFSET(x) ((char *)NULL + x)
#endif

#ifndef assert_not_null
#define assert_not_null(param) \
	assert(param != nullptr && "Parameter "## #param ##" cannot be nullptr")
#endif

#ifndef assert_null
#define assert_null(param) \
	assert(param == nullptr && "Parameter "## #param ##" is expected be nullptr")
#endif

#ifndef assert_not_implemented
#define assert_not_implemented() assert(false && "Not implemented")
#endif

#define FLOAT_EQUALS(val1, val2) (abs(val2 - val1) <= FLT_EPSILON)
#define FLOAT_NEQUALS(val1, val2) (abs(val2 - val1) > FLT_EPSILON)

#ifndef BIT
#define BIT(x) 1 << x
#endif

#ifndef BIT_ISSET
#define BIT_ISSET(a, b) (( (a) & (b) ) != 0)
#endif

#ifndef BIT_UNSET
#define BIT_UNSET(a, b) (a &= ~b)
#endif

#ifndef BIT_SET
#define BIT_SET(a, b) (a |= (b))
#endif

#ifndef BIT_ALL
#define BIT_ALL ULONG_MAX
#endif

#ifndef BIT_NONE
#define BIT_NONE 0
#endif

//
// Math constants
//

#ifndef M_E
#define M_E 2.71828182845904523536
#endif

#ifndef M_LOG2E
#define M_LOG2E 1.44269504088896340736
#endif

#ifndef M_LOG10E
#define M_LOG10E 0.434294481903251827651
#endif

#ifndef M_LN2
#define M_LN2 0.693147180559945309417
#endif

#ifndef M_LN10
#define M_LN10 2.30258509299404568402
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_F
#define M_PI_F ((float32)M_PI)
#endif

#ifndef M_PI_DIV180
#define M_PI_DIV180 0.01745329251994329576
#endif

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

#ifndef M_PI_4
#define M_PI_4 0.785398163397448309616
#endif

#ifndef M_1_PI
#define M_1_PI 0.318309886183790671538
#endif

#ifndef M_2_PI
#define M_2_PI 0.636619772367581343076
#endif

#ifndef M_1_SQRTPI
#define M_1_SQRTPI 0.564189583547756286948
#endif

#ifndef M_2_SQRTPI
#define M_2_SQRTPI 1.12837916709551257390
#endif

#ifndef M_SQRT2
#define M_SQRT2 1.41421356237309504880
#endif

#ifndef M_SQRT_2
#define M_SQRT_2 0.707106781186547524401
#endif

#ifndef ANG2RAD
// 2.0 * M_PI / 360.0
#define ANG2RAD 0.01745329251994329576922
#endif

namespace core
{
	/*!
		\brief Custom delete struct used with std::shared_ptr to delete arrays

		Example:
		{@code
			std::shared_ptr<byte> bytes(new byte[10], TDeleteArray<byte>());
		}
	*/
	template<typename T>
	struct TDeleteArray
	{
		void operator()(T const * p) {
			delete[] p;
		}
	};
}
