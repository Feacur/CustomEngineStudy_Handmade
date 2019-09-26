#define SHARED_CODE

#include <intrin.h> // __debugbreak(), SIMD
#include <stdint.h> // integer types data
#include <float.h>  // floating point types data
#include <stdio.h>  // printf
#include <string.h> // memset, memcpy
#include <math.h>   // sqrtf, sinf, cosf

// basic types
typedef int8_t   int8;  // signed char
typedef int16_t  int16; // short
typedef int32_t  int32; // int
typedef int64_t  int64; // long long

typedef uint8_t  uint8;  // unsigned char
typedef uint16_t uint16; // unsigned short
typedef uint32_t uint32; // unsigned int
typedef uint64_t uint64; // unsigned long long

typedef long          int48;  // witty (32 + 64) / 2
typedef unsigned long uint48; // witty (32 + 64) / 2

typedef char const * cstring;

// macros
#define STRINGIFY_A_VALUE(VALUE) #VALUE
#define STRINGIFY_A_MACRO(MACRO) STRINGIFY_A_VALUE(MACRO)
#define FILE_AND_LINE __FILE__ ":" STRINGIFY_A_MACRO(__LINE__)
#define C_ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

#undef min
#undef max

#if !defined(__cplusplus) // hide cpp stuff
	#define constexpr
	#define thread_local
#endif // defined(__cplusplus) // hide cpp stuff

// platform
#if defined(__cplusplus) // platform
	#define API_C extern "C"
	#define API_C_BLOCK_START extern "C" {
	#define API_C_BLOCK_END }
#else
	#define API_C
	#define API_C_BLOCK_START
	#define API_C_BLOCK_END
#endif //  defined(__cplusplus) // platform

#if defined(PLATFORM_WINDOWS)
	#define CODE_BREAK() __debugbreak()
	#if defined(BUILD_DLL)
		#define API_DLL __declspec(dllexport)
		#define API_TEMPLATE
	#else
		#define API_DLL __declspec(dllimport)
		#define API_TEMPLATE extern
	#endif
#else
	#error supported platforms: Windows
#endif

// logging
#if defined(SHIPPING)
	#define LOG_TRACE(MESSAGE)
	#define LOG_WARNING(MESSAGE)
	#define LOG_ERROR(MESSAGE)
	#define ASSERT_TRUE(STATEMENT, MESSAGE)
#else
	inline void logger_message(cstring type, cstring location, cstring message) {
		printf("%s: %s\n\t---> %s\n", type, message, location);
	}
	#define LOG_TRACE(MESSAGE)   logger_message("TRACE",   FILE_AND_LINE, MESSAGE)
	#define LOG_WARNING(MESSAGE) logger_message("WARNING", FILE_AND_LINE, MESSAGE)
	#define LOG_ERROR(MESSAGE)   logger_message("ERROR",   FILE_AND_LINE, MESSAGE)
	#define ASSERT_TRUE(STATEMENT, MESSAGE) { if(!(STATEMENT)) { LOG_ERROR(MESSAGE); CODE_BREAK(); } }
#endif

// traits

#if defined(__cplusplus) // meta
	#if defined(_MSC_VER)
		namespace meta {
			template<typename T>
			struct underlying_type {
				typedef __underlying_type(T) type;
			};
			
			template<typename T>
			struct is_enum {
				static bool const value = __is_enum(T);
			};
		}
		#define UNDERLYING_TYPE_META(T, U)
		#define IS_ENUM_META(T)
	#else
		namespace meta {
			template<typename T> struct underlying_type { };
			template<typename T> struct is_enum { };
		}
		#define UNDERLYING_TYPE_META(T, U)\
		namespace meta { template<> struct underlying_type<T> { typedef U type; }; }

		#define IS_ENUM_META(T)\
		namespace meta { template<> struct is_enum<T> { static bool const value = true; }; }
	#endif
#endif // defined(__cplusplus) // meta
