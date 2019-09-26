#define SHARED_MATH

#if !defined(SHARED_CODE)
	#error include "shared/code.h"
#endif

struct Vector2 {
	union {
		struct { float x, y; };
		float data[2];
	};

#if defined(__cplusplus) // Vector2
	constexpr inline float operator[](size_t i) { return data[i]; }
	constexpr inline float const & operator[](size_t i) const { return data[i]; }
#endif // defined(__cplusplus) // Vector2
};

struct Vector3 {
	union {
		struct { Vector2 xy; float z; };
		struct { float x; Vector2 yz; };
		struct { float x, y, z; };
		float data[3];
	};

#if defined(__cplusplus) // Vector3
	constexpr inline float operator[](size_t i) { return data[i]; }
	constexpr inline float const & operator[](size_t i) const { return data[i]; }
#endif // defined(__cplusplus) // Vector3
};

struct Vector4 {
	union {
		struct { Vector3 xyz; float w; };
		struct { float x; Vector3 yzw; };
		struct { Vector2 xy; Vector2 zw; };
		struct { float x; Vector2 yz; float w; };
		struct { float x, y, z, w; };
		float data[4];
	};

#if defined(__cplusplus) // Vector4
	constexpr inline float operator[](size_t i) { return data[i]; }
	constexpr inline float const & operator[](size_t i) const { return data[i]; }
#endif // defined(__cplusplus) // Vector4
};

struct Vector2i {
	union {
		struct { int32 x, y; };
		int32 data[2];
	};
	
#if defined(__cplusplus) // Vector2i
	constexpr inline int32 & operator[](size_t i) { return data[i]; }
	constexpr inline int32 const & operator[](size_t i) const { return data[i]; }
#endif // defined(__cplusplus) // Vector2i
};

struct Vector3i {
	union {
		struct { Vector2i xy; int32 z; };
		struct { int32 x; Vector2i yz; };
		struct { int32 x, y, z; };
		int32 data[3];
	};
	
#if defined(__cplusplus) // Vector3i
	constexpr inline int32 & operator[](size_t i) { return data[i]; }
	constexpr inline int32 const & operator[](size_t i) const { return data[i]; }
#endif // defined(__cplusplus) // Vector3i
};

// typedef Vector2i Complexi;
using Complexi = Vector2i;

// typedef Vector2 Complex;
using Complex = Vector2;

// typedef Vector4 Quaternion;
using Quaternion = Vector4;

constexpr inline Vector2i vec2i(int32 x, int32 y) { return {x, y}; }
constexpr inline Vector3i vec3i(int32 x, int32 y, int32 z) { return {x, y, z}; }

constexpr inline Vector2 vec2(float x, float y) { return {x, y}; }
constexpr inline Vector3 vec3(float x, float y, float z) { return {x, y, z}; }
constexpr inline Vector4 vec4(float x, float y, float z, float w) { return {x, y, z, w}; }

// @Note
// vector structs have duplicate names in them
// though it seems to be OK, as their layout matches
// still, debugger draws them ugly

static float const epsilon    = 0.00001f;
static float const pi         = 3.14159265359f;
static float const tau        = pi * 2; // 6.28318530718f
static float const deg_to_rad = pi / 180; // 0.01745329251f

// with 64 bit integers compiler has no chance promoting to a larger storage type
// thus naive "return (value * numerator) / denominator" wont't work properly
// neither "return (value / denominator) * numerator" would, obviously
// substitute "value" == "(value / denominator) * denominator + (value % denominator)", simplify

// thanks: https://github.com/rust-lang/rust/blob/master/src/libstd/sys_common/mod.rs

#define MUL_DIV_IMPL(T)\
constexpr inline T mul_div(T value, T numerator, T denominator) {\
	T a = value / denominator;\
	T b = value % denominator;\
	return a * numerator + b * numerator / denominator;\
}

MUL_DIV_IMPL(int8)
MUL_DIV_IMPL(int16)
MUL_DIV_IMPL(int32)
MUL_DIV_IMPL(int64)
MUL_DIV_IMPL(uint8)
MUL_DIV_IMPL(uint16)
MUL_DIV_IMPL(uint32)
MUL_DIV_IMPL(uint64)

MUL_DIV_IMPL(int48)
MUL_DIV_IMPL(uint48)
#undef MUL_DIV_IMPL

//
//
//


#define SIGN_NON_ZERO_IMPL(T)\
constexpr inline T sign_non_zero(T value) {\
	return (value < 0) ? -1 : 1;\
}

#define SIGN_IMPL(T)\
constexpr inline T sign(T value) {\
	if (value < 0) { return -1; }\
	if (value > 0) { return 1; }\
	return 0;\
}

#define MIN_IMPL(T)\
constexpr inline T min(T first, T second) {\
	return (first < second) ? first : second;\
}

#define MAX_IMPL(T)\
constexpr inline T max(T first, T second) {\
	return (first > second) ? first : second;\
}

SIGN_NON_ZERO_IMPL(int8)
SIGN_NON_ZERO_IMPL(int16)
SIGN_NON_ZERO_IMPL(int32)
SIGN_NON_ZERO_IMPL(int64)

SIGN_NON_ZERO_IMPL(int48)
#undef SIGN_NON_ZERO_IMPL

SIGN_IMPL(int8)
SIGN_IMPL(int16)
SIGN_IMPL(int32)
SIGN_IMPL(int64)

SIGN_IMPL(int48)
#undef SIGN_IMPL

MIN_IMPL(int8)
MIN_IMPL(int16)
MIN_IMPL(int32)
MIN_IMPL(int64)
MIN_IMPL(uint8)
MIN_IMPL(uint16)
MIN_IMPL(uint32)
MIN_IMPL(uint64)

MIN_IMPL(int48)
MIN_IMPL(uint48)
#undef MIN_IMPL

MAX_IMPL(int8)
MAX_IMPL(int16)
MAX_IMPL(int32)
MAX_IMPL(int64)
MAX_IMPL(uint8)
MAX_IMPL(uint16)
MAX_IMPL(uint32)
MAX_IMPL(uint64)

MAX_IMPL(int48)
MAX_IMPL(uint48)
#undef MAX_IMPL

//
// int32
//

constexpr inline int32 interpolate(int32 from, int32 to, int32 mul, int32 div) {
	return from + mul_div((to - from), mul, div);
}

constexpr inline int32 absolute(int32 value) {
	return value >= 0 ? value : -value;
}

constexpr inline int32 round_up_with_step(int32 value, int32 step) {
	int32 reminder = (value % step);
	return (reminder < 1) ? value : (value + (step - reminder));
}

//
// float
//

constexpr inline float sign_non_zero(float value) {
	return (value < 0.0f) ? -1.0f : 1.0f;
}

constexpr inline float sign(float value) {
	if (value < 0) { return -1.0f; }
	if (value > 0) { return 1.0f; }
	return 0.0f;
}

constexpr inline float min(float first, float second) {
	return (first < second) ? first : second;
}

constexpr inline float max(float first, float second) {
	return (first > second) ? first : second;
}

constexpr inline float interpolate(float from, float to, float fraction) {
	return from + (to - from) * fraction;
}

constexpr inline float absolute(float value) {
	return value >= 0 ? value : -value;
}

inline float square_root(float value) {
	return sqrtf(value);
}

inline float sine(float value) {
	return sinf(value);
}

inline float cosine(float value) {
	return cosf(value);
}

inline float tangent(float value) {
	return tanf(value);
}

//
// Mix
//

constexpr inline float uint8_to_fraction(uint8 value) {
	return value / 255.f;
}

constexpr inline float uint16_to_fraction(uint16 value) {
	return value / 65535.f;
}

constexpr inline float int8_to_fraction(int8 value) {
	return (value > 0) ? (value / 127.f) : (value / 128.f);
}

constexpr inline float int16_to_fraction(int16 value) {
	return (value > 0) ? (value / 32767.f) : (value / 32768.f);
}

//
//
//

constexpr inline int32 cross_product(Vector2i first, Vector2i second) {
	return first.x * second.y - first.y * second.x;
}

constexpr inline Vector3i cross_product(Vector3i first, Vector3i second) {
	return {
		first.y * second.z - first.z * second.y,
		first.z * second.x - first.x * second.z,
		first.x * second.y - first.y * second.x
	};
}

constexpr inline float cross_product(Vector2 first, Vector2 second) {
	return first.x * second.y - first.y * second.x;
}

constexpr inline Vector3 cross_product(Vector3 first, Vector3 second) {
	return {
		first.y * second.z - first.z * second.y,
		first.z * second.x - first.x * second.z,
		first.x * second.y - first.y * second.x
	};
}

//
// Vector2i routines
// Applications: 2d position
//

#if defined(__cplusplus) // operator Vector2i
constexpr inline bool operator==(Vector2i first, Vector2i second) {
	return (first.x == second.x)
	    && (first.y == second.y);
}

constexpr inline bool operator!=(Vector2i first, Vector2i second) {
	return (first.x != second.x)
	    || (first.y != second.y);
}

constexpr inline Vector2i operator+(Vector2i first, Vector2i second) {
	return {
		first.x + second.x,
		first.y + second.y
	};
}

constexpr inline Vector2i operator+(Vector2i first, int32 second) {
	return {
		first.x + second,
		first.y + second
	};
}

constexpr inline Vector2i operator-(Vector2i first, Vector2i second) {
	return {
		first.x - second.x,
		first.y - second.y
	};
}

constexpr inline Vector2i operator-(Vector2i first, int32 second) {
	return {
		first.x - second,
		first.y - second
	};
}

constexpr inline Vector2i operator-(Vector2i value) {
	return {
		-value.x,
		-value.y
	};
}

constexpr inline Vector2i operator*(Vector2i first, int32 second) {
	return {
		first.x * second,
		first.y * second
	};
}

constexpr inline Vector2i operator*(Vector2i first, Vector2i second) {
	return {
		first.x * second.x,
		first.y * second.y
	};
}

constexpr inline Vector2i operator/(Vector2i first, int32 second) {
	return {
		first.x / second,
		first.y / second
	};
}

constexpr inline Vector2i operator/(Vector2i first, Vector2i second) {
	return {
		first.x / second.x,
		first.y / second.y
	};
}

constexpr inline Vector2i operator%(Vector2i first, int32 second) {
	return {
		first.x % second,
		first.y % second
	};
}

constexpr inline Vector2i operator%(Vector2i first, Vector2i second) {
	return {
		first.x % second.x,
		first.y % second.y
	};
}

constexpr inline Vector2i & operator+=(Vector2i & first, Vector2i const & second) {
	first.x += second.x;
	first.y += second.y;
	return first;
}

constexpr inline Vector2i & operator+=(Vector2i & first, int32 second) {
	first.x += second;
	first.y += second;
	return first;
}

constexpr inline Vector2i & operator-=(Vector2i & first, Vector2i const & second) {
	first.x -= second.x;
	first.y -= second.y;
	return first;
}

constexpr inline Vector2i & operator-=(Vector2i & first, int32 second) {
	first.x -= second;
	first.y -= second;
	return first;
}

constexpr inline Vector2i & operator*=(Vector2i & first, Vector2i const & second) {
	first.x *= second.x;
	first.y *= second.y;
	return first;
}

constexpr inline Vector2i & operator*=(Vector2i & first, int32 second) {
	first.x *= second;
	first.y *= second;
	return first;
}

constexpr inline Vector2i & operator/=(Vector2i & first, Vector2i const & second) {
	first.x /= second.x;
	first.y /= second.y;
	return first;
}

constexpr inline Vector2i & operator/=(Vector2i & first, int32 second) {
	first.x /= second;
	first.y /= second;
	return first;
}

constexpr inline Vector2i & operator%=(Vector2i & first, Vector2i const & second) {
	first.x %= second.x;
	first.y %= second.y;
	return first;
}

constexpr inline Vector2i & operator%=(Vector2i & first, int32 second) {
	first.x %= second;
	first.y %= second;
	return first;
}
#endif // defined(__cplusplus) // operator Vector2i

constexpr inline int32 dot_product(Vector2i first, Vector2i second) {
	return first.x * second.x
	     + first.y * second.y;
}

constexpr inline Vector2i min(Vector2i first, Vector2i second) {
	return {
		min(first.x, second.x),
		min(first.y, second.y)
	};
}

constexpr inline Vector2i max(Vector2i first, Vector2i second) {
	return {
		max(first.x, second.x),
		max(first.y, second.y)
	};
}

constexpr inline int32 min(Vector2i value) {
	return min(value.x, value.y);
}

constexpr inline int32 max(Vector2i value) {
	return max(value.x, value.y);
}

constexpr inline Vector2i absolute(Vector2i value) {
	return {
		absolute(value.x),
		absolute(value.y)
	};
}

constexpr inline Vector2i sign(Vector2i value) {
	return {
		sign(value.x),
		sign(value.y)
	};
}

constexpr inline Vector2i interpolate(Vector2i from, Vector2i to, int32 mul, int32 div) {
	return {
		interpolate(from.x, to.x, mul, div),
		interpolate(from.y, to.y, mul, div)
	};
}

//
// Vector3i routines
// Applications: 3d position
//

#if defined(__cplusplus) // operator Vector3i
constexpr inline bool operator==(Vector3i first, Vector3i second) {
	return (first.x == second.x)
	    && (first.y == second.y)
	    && (first.z == second.z);
}

constexpr inline bool operator!=(Vector3i first, Vector3i second) {
	return (first.x != second.x)
	    || (first.y != second.y)
	    || (first.z != second.z);
}

constexpr inline Vector3i operator+(Vector3i first, Vector3i second) {
	return {
		first.x + second.x,
		first.y + second.y,
		first.z + second.z
	};
}

constexpr inline Vector3i operator+(Vector3i first, int32 second) {
	return {
		first.x + second,
		first.y + second,
		first.z + second
	};
}

constexpr inline Vector3i operator-(Vector3i first, Vector3i second) {
	return {
		first.x - second.x,
		first.y - second.y,
		first.z - second.z
	};
}

constexpr inline Vector3i operator-(Vector3i first, int32 second) {
	return {
		first.x - second,
		first.y - second,
		first.z - second
	};
}

constexpr inline Vector3i operator-(Vector3i value) {
	return {
		-value.x,
		-value.y,
		-value.z
	};
}

constexpr inline Vector3i operator*(Vector3i first, int32 second) {
	return {
		first.x * second,
		first.y * second,
		first.z * second
	};
}

constexpr inline Vector3i operator*(Vector3i first, Vector3i second) {
	return {
		first.x * second.x,
		first.y * second.y,
		first.z * second.z
	};
}

constexpr inline Vector3i operator/(Vector3i first, int32 second) {
	return {
		first.x / second,
		first.y / second,
		first.z / second
	};
}

constexpr inline Vector3i operator/(Vector3i first, Vector3i second) {
	return {
		first.x / second.x,
		first.y / second.y,
		first.z / second.z
	};
}

constexpr inline Vector3i operator%(Vector3i first, int32 second) {
	return {
		first.x % second,
		first.y % second,
		first.z % second
	};
}

constexpr inline Vector3i operator%(Vector3i first, Vector3i second) {
	return {
		first.x % second.x,
		first.y % second.y,
		first.z % second.z
	};
}

constexpr inline Vector3i & operator+=(Vector3i & first, Vector3i const & second) {
	first.x += second.x;
	first.y += second.y;
	first.z += second.z;
	return first;
}

constexpr inline Vector3i & operator+=(Vector3i & first, int32 second) {
	first.x += second;
	first.y += second;
	first.z += second;
	return first;
}

constexpr inline Vector3i & operator-=(Vector3i & first, Vector3i const & second) {
	first.x -= second.x;
	first.y -= second.y;
	first.z -= second.z;
	return first;
}

constexpr inline Vector3i & operator-=(Vector3i & first, int32 second) {
	first.x -= second;
	first.y -= second;
	first.z -= second;
	return first;
}

constexpr inline Vector3i & operator*=(Vector3i & first, Vector3i const & second) {
	first.x *= second.x;
	first.y *= second.y;
	first.z *= second.z;
	return first;
}

constexpr inline Vector3i & operator*=(Vector3i & first, int32 second) {
	first.x *= second;
	first.y *= second;
	first.z *= second;
	return first;
}

constexpr inline Vector3i & operator/=(Vector3i & first, Vector3i const & second) {
	first.x /= second.x;
	first.y /= second.y;
	first.z /= second.z;
	return first;
}

constexpr inline Vector3i & operator/=(Vector3i & first, int32 second) {
	first.x /= second;
	first.y /= second;
	first.z /= second;
	return first;
}

constexpr inline Vector3i & operator%=(Vector3i & first, Vector3i const & second) {
	first.x %= second.x;
	first.y %= second.y;
	first.z %= second.z;
	return first;
}

constexpr inline Vector3i & operator%=(Vector3i & first, int32 second) {
	first.x %= second;
	first.y %= second;
	first.z %= second;
	return first;
}
#endif // defined(__cplusplus) // operator Vector3i

constexpr inline int32 dot_product(Vector3i first, Vector3i second) {
	return first.x * second.x
	     + first.y * second.y
	     + first.z * second.z;
}

constexpr inline Vector3i min(Vector3i first, Vector3i second) {
	return {
		min(first.x, second.x),
		min(first.y, second.y),
		min(first.z, second.z)
	};
}

constexpr inline Vector3i max(Vector3i first, Vector3i second) {
	return {
		max(first.x, second.x),
		max(first.y, second.y),
		max(first.z, second.z)
	};
}

constexpr inline int32 min(Vector3i value) {
	return min(min(value.x, value.y), value.z);
}

constexpr inline int32 max(Vector3i value) {
	return max(max(value.x, value.y), value.z);
}

constexpr inline Vector3i absolute(Vector3i value) {
	return {
		absolute(value.x),
		absolute(value.y),
		absolute(value.z)
	};
}

constexpr inline Vector3i sign(Vector3i value) {
	return {
		sign(value.x),
		sign(value.y),
		sign(value.z)
	};
}

constexpr inline Vector3i interpolate(Vector3i from, Vector3i to, int32 mul, int32 div) {
	return {
		interpolate(from.x, to.x, mul, div),
		interpolate(from.y, to.y, mul, div),
		interpolate(from.z, to.z, mul, div)
	};
}

//
// Vector2 routines
// Applications: 2d position, 2d rotation as complex number, 2d scale, 2d direction
//

#if defined(__cplusplus) // operator Vector2
constexpr inline bool operator==(Vector2 first, Vector2 second) {
	return (first.x == second.x)
	    && (first.y == second.y);
}

constexpr inline bool operator!=(Vector2 first, Vector2 second) {
	return (first.x != second.x)
	    || (first.y != second.y);
}

constexpr inline Vector2 operator+(Vector2 first, Vector2 second) {
	return {
		first.x + second.x,
		first.y + second.y
	};
}

constexpr inline Vector2 operator+(Vector2 first, float second) {
	return {
		first.x + second,
		first.y + second
	};
}

constexpr inline Vector2 operator-(Vector2 first, Vector2 second) {
	return {
		first.x - second.x,
		first.y - second.y
	};
}

constexpr inline Vector2 operator-(Vector2 first, float second) {
	return {
		first.x - second,
		first.y - second
	};
}

constexpr inline Vector2 operator-(Vector2 value) {
	return {
		-value.x,
		-value.y
	};
}

constexpr inline Vector2 operator*(Vector2 first, float second) {
	return {
		first.x * second,
		first.y * second
	};
}

constexpr inline Vector2 operator*(Vector2 first, Vector2 second) {
	return {
		first.x * second.x,
		first.y * second.y
	};
}

constexpr inline Vector2 operator/(Vector2 first, float second) {
	return {
		first.x / second,
		first.y / second
	};
}

constexpr inline Vector2 operator/(Vector2 first, Vector2 second) {
	return {
		first.x / second.x,
		first.y / second.y
	};
}

constexpr inline Vector2 & operator+=(Vector2 & first, Vector2 const & second) {
	first.x += second.x;
	first.y += second.y;
	return first;
}

constexpr inline Vector2 & operator+=(Vector2 & first, float second) {
	first.x += second;
	first.y += second;
	return first;
}

constexpr inline Vector2 & operator-=(Vector2 & first, Vector2 const & second) {
	first.x -= second.x;
	first.y -= second.y;
	return first;
}

constexpr inline Vector2 & operator-=(Vector2 & first, float second) {
	first.x -= second;
	first.y -= second;
	return first;
}

constexpr inline Vector2 & operator*=(Vector2 & first, Vector2 const & second) {
	first.x *= second.x;
	first.y *= second.y;
	return first;
}

constexpr inline Vector2 & operator*=(Vector2 & first, float second) {
	first.x *= second;
	first.y *= second;
	return first;
}

constexpr inline Vector2 & operator/=(Vector2 & first, Vector2 const & second) {
	first.x /= second.x;
	first.y /= second.y;
	return first;
}

constexpr inline Vector2 & operator/=(Vector2 & first, float second) {
	first.x /= second;
	first.y /= second;
	return first;
}
#endif // defined(__cplusplus) // operator Vector2

constexpr inline float dot_product(Vector2 first, Vector2 second) {
	return first.x * second.x
	     + first.y * second.y;
}

constexpr inline Vector2 min(Vector2 first, Vector2 second) {
	return {
		min(first.x, second.x),
		min(first.y, second.y)
	};
}

constexpr inline Vector2 max(Vector2 first, Vector2 second) {
	return {
		max(first.x, second.x),
		max(first.y, second.y)
	};
}

constexpr inline float min(Vector2 value) {
	return min(value.x, value.y);
}

constexpr inline float max(Vector2 value) {
	return max(value.x, value.y);
}

constexpr inline Vector2 absolute(Vector2 value) {
	return {
		absolute(value.x),
		absolute(value.y)
	};
}

constexpr inline Vector2 sign(Vector2 value) {
	return {
		sign(value.x),
		sign(value.y)
	};
}

constexpr inline Vector2 interpolate(Vector2 from, Vector2 to, float fraction) {
	return {
		interpolate(from.x, to.x, fraction),
		interpolate(from.y, to.y, fraction)
	};
}

inline Vector2 square_root(Vector2 value) {
	return {
		square_root(value.x),
		square_root(value.y)
	};
}

inline Vector2 sine(Vector2 value) {
	return {
		sine(value.x),
		sine(value.y)
	};
}

inline Vector2 cosine(Vector2 value) {
	return {
		cosine(value.x),
		cosine(value.y)
	};
}

//
// Vector3 routines
// Applications: 3d position, 3d rotation as euler angles, 3d scale, 3d direction
//

#if defined(__cplusplus) // operator Vector3
constexpr inline bool operator==(Vector3 first, Vector3 second) {
	return (first.x == second.x)
	    && (first.y == second.y)
	    && (first.z == second.z);
}

constexpr inline bool operator!=(Vector3 first, Vector3 second) {
	return (first.x != second.x)
	    || (first.y != second.y)
	    || (first.z != second.z);
}

constexpr inline Vector3 operator+(Vector3 first, Vector3 second) {
	return {
		first.x + second.x,
		first.y + second.y,
		first.z + second.z
	};
}

constexpr inline Vector3 operator+(Vector3 first, float second) {
	return {
		first.x + second,
		first.y + second,
		first.z + second
	};
}

constexpr inline Vector3 operator-(Vector3 first, Vector3 second) {
	return {
		first.x - second.x,
		first.y - second.y,
		first.z - second.z
	};
}

constexpr inline Vector3 operator-(Vector3 first, float second) {
	return {
		first.x - second,
		first.y - second,
		first.z - second
	};
}

constexpr inline Vector3 operator-(Vector3 value) {
	return {
		-value.x,
		-value.y,
		-value.z
	};
}

constexpr inline Vector3 operator*(Vector3 first, float second) {
	return {
		first.x * second,
		first.y * second,
		first.z * second
	};
}

constexpr inline Vector3 operator*(Vector3 first, Vector3 second) {
	return {
		first.x * second.x,
		first.y * second.y,
		first.z * second.z
	};
}

constexpr inline Vector3 operator/(Vector3 first, float second) {
	return {
		first.x / second,
		first.y / second,
		first.z / second
	};
}

constexpr inline Vector3 operator/(Vector3 first, Vector3 second) {
	return {
		first.x / second.x,
		first.y / second.y,
		first.z / second.z
	};
}

constexpr inline Vector3 & operator+=(Vector3 & first, Vector3 const & second) {
	first.x += second.x;
	first.y += second.y;
	first.z += second.z;
	return first;
}

constexpr inline Vector3 & operator+=(Vector3 & first, float second) {
	first.x += second;
	first.y += second;
	first.z += second;
	return first;
}

constexpr inline Vector3 & operator-=(Vector3 & first, Vector3 const & second) {
	first.x -= second.x;
	first.y -= second.y;
	first.z -= second.z;
	return first;
}

constexpr inline Vector3 & operator-=(Vector3 & first, float second) {
	first.x -= second;
	first.y -= second;
	first.z -= second;
	return first;
}

constexpr inline Vector3 & operator*=(Vector3 & first, Vector3 const & second) {
	first.x *= second.x;
	first.y *= second.y;
	first.z *= second.z;
	return first;
}

constexpr inline Vector3 & operator*=(Vector3 & first, float second) {
	first.x *= second;
	first.y *= second;
	first.z *= second;
	return first;
}

constexpr inline Vector3 & operator/=(Vector3 & first, Vector3 const & second) {
	first.x /= second.x;
	first.y /= second.y;
	first.z /= second.z;
	return first;
}

constexpr inline Vector3 & operator/=(Vector3 & first, float second) {
	first.x /= second;
	first.y /= second;
	first.z /= second;
	return first;
}
#endif // defined(__cplusplus) // operator Vector3

constexpr inline float dot_product(Vector3 first, Vector3 second) {
	return first.x * second.x
	     + first.y * second.y
	     + first.z * second.z;
}

constexpr inline Vector3 min(Vector3 first, Vector3 second) {
	return {
		min(first.x, second.x),
		min(first.y, second.y),
		min(first.z, second.z)
	};
}

constexpr inline Vector3 max(Vector3 first, Vector3 second) {
	return {
		max(first.x, second.x),
		max(first.y, second.y),
		max(first.z, second.z)
	};
}

constexpr inline float min(Vector3 value) {
	return min(min(value.x, value.y), value.z);
}

constexpr inline float max(Vector3 value) {
	return max(max(value.x, value.y), value.z);
}

constexpr inline Vector3 absolute(Vector3 value) {
	return {
		absolute(value.x),
		absolute(value.y),
		absolute(value.z)
	};
}

constexpr inline Vector3 sign(Vector3 value) {
	return {
		sign(value.x),
		sign(value.y),
		sign(value.z)
	};
}

constexpr inline Vector3 interpolate(Vector3 from, Vector3 to, float fraction) {
	return {
		interpolate(from.x, to.x, fraction),
		interpolate(from.y, to.y, fraction),
		interpolate(from.z, to.z, fraction)
	};
}

inline Vector3 square_root(Vector3 value) {
	return {
		square_root(value.x),
		square_root(value.y),
		square_root(value.z)
	};
}

inline Vector3 sine(Vector3 value) {
	return {
		sine(value.x),
		sine(value.y),
		sine(value.z)
	};
}

inline Vector3 cosine(Vector3 value) {
	return {
		cosine(value.x),
		cosine(value.y),
		cosine(value.z)
	};
}

//
// Vector4 routines
// Applications: 3d rotation as quaternion, color
//

#if defined(__cplusplus) // operator Vector4
constexpr inline bool operator==(Vector4 first, Vector4 second) {
	return (first.x == second.x)
	    && (first.y == second.y)
	    && (first.z == second.z)
	    && (first.w == second.w);
}

constexpr inline bool operator!=(Vector4 first, Vector4 second) {
	return (first.x != second.x)
	    || (first.y != second.y)
	    || (first.z != second.z)
	    || (first.w != second.w);
}

constexpr inline Vector4 operator+(Vector4 first, Vector4 second) {
	return {
		first.x + second.x,
		first.y + second.y,
		first.z + second.z,
		first.w + second.w
	};
}

constexpr inline Vector4 operator+(Vector4 first, float second) {
	return {
		first.x + second,
		first.y + second,
		first.z + second,
		first.w + second
	};
}

constexpr inline Vector4 operator-(Vector4 first, Vector4 second) {
	return {
		first.x - second.x,
		first.y - second.y,
		first.z - second.z,
		first.w - second.w
	};
}

constexpr inline Vector4 operator-(Vector4 first, float second) {
	return {
		first.x - second,
		first.y - second,
		first.z - second,
		first.w - second
	};
}

constexpr inline Vector4 operator-(Vector4 value) {
	return {
		-value.x,
		-value.y,
		-value.z,
		-value.w
	};
}

constexpr inline Vector4 operator*(Vector4 first, float second) {
	return {
		first.x * second,
		first.y * second,
		first.z * second,
		first.w * second
	};
}

constexpr inline Vector4 operator*(Vector4 first, Vector4 second) {
	return {
		first.x * second.x,
		first.y * second.y,
		first.z * second.z,
		first.w * second.w
	};
}

constexpr inline Vector4 operator/(Vector4 first, float second) {
	return {
		first.x / second,
		first.y / second,
		first.z / second,
		first.w / second
	};
}

constexpr inline Vector4 operator/(Vector4 first, Vector4 second) {
	return {
		first.x / second.x,
		first.y / second.y,
		first.z / second.z,
		first.w / second.w
	};
}

constexpr inline Vector4 & operator+=(Vector4 & first, Vector4 const & second) {
	first.x += second.x;
	first.y += second.y;
	first.z += second.z;
	first.w += second.w;
	return first;
}

constexpr inline Vector4 & operator+=(Vector4 & first, float second) {
	first.x += second;
	first.y += second;
	first.z += second;
	first.w += second;
	return first;
}

constexpr inline Vector4 & operator-=(Vector4 & first, Vector4 const & second) {
	first.x -= second.x;
	first.y -= second.y;
	first.z -= second.z;
	first.w -= second.w;
	return first;
}

constexpr inline Vector4 & operator-=(Vector4 & first, float second) {
	first.x -= second;
	first.y -= second;
	first.z -= second;
	first.w -= second;
	return first;
}

constexpr inline Vector4 & operator*=(Vector4 & first, Vector4 const & second) {
	first.x *= second.x;
	first.y *= second.y;
	first.z *= second.z;
	first.w *= second.w;
	return first;
}

constexpr inline Vector4 & operator*=(Vector4 & first, float second) {
	first.x *= second;
	first.y *= second;
	first.z *= second;
	first.w *= second;
	return first;
}

constexpr inline Vector4 & operator/=(Vector4 & first, Vector4 const & second) {
	first.x /= second.x;
	first.y /= second.y;
	first.z /= second.z;
	first.w /= second.w;
	return first;
}

constexpr inline Vector4 & operator/=(Vector4 & first, float second) {
	first.x /= second;
	first.y /= second;
	first.z /= second;
	first.w /= second;
	return first;
}
#endif // defined(__cplusplus) // operator Vector4

constexpr inline float dot_product(Vector4 first, Vector4 second) {
	return first.x * second.x
	     + first.y * second.y
	     + first.z * second.z
	     + first.w * second.w;
}

constexpr inline Vector4 min(Vector4 first, Vector4 second) {
	return {
		min(first.x, second.x),
		min(first.y, second.y),
		min(first.z, second.z),
		min(first.w, second.w)
	};
}

constexpr inline Vector4 max(Vector4 first, Vector4 second) {
	return {
		max(first.x, second.x),
		max(first.y, second.y),
		max(first.z, second.z),
		max(first.w, second.w)
	};
}

constexpr inline float min(Vector4 value) {
	return min(min(min(value.x, value.y), value.z), value.w);
}

constexpr inline float max(Vector4 value) {
	return max(max(max(value.x, value.y), value.z), value.w);
}

constexpr inline Vector4 absolute(Vector4 value) {
	return {
		absolute(value.x),
		absolute(value.y),
		absolute(value.z),
		absolute(value.w)
	};
}

constexpr inline Vector4 sign(Vector4 value) {
	return {
		sign(value.x),
		sign(value.y),
		sign(value.z),
		sign(value.w)
	};
}

constexpr inline Vector4 interpolate(Vector4 from, Vector4 to, float fraction) {
	return {
		interpolate(from.x, to.x, fraction),
		interpolate(from.y, to.y, fraction),
		interpolate(from.z, to.z, fraction),
		interpolate(from.w, to.w, fraction)
	};
}

inline Vector4 square_root(Vector4 value) {
	return {
		square_root(value.x),
		square_root(value.y),
		square_root(value.z),
		square_root(value.w)
	};
}

inline Vector4 sine(Vector4 value) {
	return {
		sine(value.x),
		sine(value.y),
		sine(value.z),
		sine(value.w)
	};
}

inline Vector4 cosine(Vector4 value) {
	return {
		cosine(value.x),
		cosine(value.y),
		cosine(value.z),
		cosine(value.w)
	};
}

//
// Complexi
// code assumes normalized values
//

constexpr inline Complexi complex_reciprocal(Complexi value) {
	return {value.x, -value.y};
}

constexpr inline Complexi complex_multiply(Complexi first, Complexi second) {
	return {
		first.x * second.x - first.y * second.y,
		first.x * second.y + first.y * second.x
	};
}

constexpr inline Complexi complex_rotate_vector(Complexi complex, Vector2i vector) {
	return complex_multiply(complex, vector);
}

//
// Complex
// code assumes normalized values
//

/*
Complex number math representation:
complex = x + y * i
complex = e ^ (angle * i)
complex = cos(angle) + i * sin(angle)

i * i = -1

Rotation formula (by angle A)
> > V' = e^(A*i) * V
*/

inline Complex complex_from_radians(float radians) {
	return {cosine(radians), sine(radians)};
}

constexpr inline Complex complex_reciprocal(Complex value) {
	return {value.x, -value.y};
}

constexpr inline Complex complex_multiply(Complex first, Complex second) {
	return {
		first.x * second.x - first.y * second.y,
		first.x * second.y + first.y * second.x
	};
}

constexpr inline Complex complex_rotate_vector(Complex complex, Vector2 vector) {
	return complex_multiply(complex, vector);
}

//
// Quaternion
// code assumes normalized values
//

/*
Quaternion math representation:
quaternion = (w + x * i) + (y + z * i) * j
quaternion = w + (x * i) + (y * j) + (z * k)
quaternion = e ^ (angle * axis)
quaternion = cos(angle) + axis * sin(angle)

(i * i) = (j * j) = (k * k) = (i * j * k) = -1

(i * j) = -(j * i) = k
(j * k) = -(k * j) = i
(k * i) = -(i * k) = j

Vector operations:
> > V = V_par + V_perp
> > NxV_par == 0
> > NxV_perp == NxV

Rodrigues' rotation formula (by angle A around axis N)
> V is parallel to axis N
> > V_par' = V_par

> V is perpendicular to axis N
> > V_perp' = V_perp * cos(A) + (NxV_perp) * sin(A)

> V is arbitrary to axis N
> > V = V_par + V_perp
> > V' = V_par + V_perp'
> > V' = V * cos(A) + (NxV) * sin(A) + N*(N*V) * (1 - cos(A))

> As you see, it's a bit cumbersome and requires trigonometric operations

Quaternion rotation formula (by angle A around axis N)
> V is parallel to axis N
> > V_par' = V_par
> > e^(A*N) * V_par == V_par * e^(A*N)

> V is perpendicular to axis N
> > V_perp' = e^(A*N) * V_perp
> > e^(A*N) * V_perp == V_perp * e^(-A*N)

> V is arbitrary to axis N
> > V = V_par + V_perp
> > V' = V_par + V_perp'
> > V' = e^(A*N/2) * V * e^(-A*N/2)

> As you see, it's concise and avoids trigonometric operations
> > also that's why we specifically use (half_radians = euler_radians / 2) in the code
*/

inline Quaternion quaternion_from_axis(Vector3 axis, float radians) {
	float half_radians = radians / 2;
	float s = sine(half_radians);
	float c = cosine(half_radians);
	return {axis.x * s, axis.y * s, axis.z * s, c};
}

constexpr inline Quaternion quaternion_multiply(Quaternion first, Quaternion second) {
	return {
		cross_product(first.xyz, second.xyz) + first.xyz * second.w + second.xyz * first.w,
		first.w * second.w - dot_product(first.xyz, second.xyz)
	};
}

/*
This code is a result of expanding following expression,
excluding stuff negated by multiplication with zero

return quaternion_multiply(
	quaternion_multiply(
		quaternion_from_axis({0, 1, 0}, euler_radians.y)
		quaternion_from_axis({1, 0, 0}, euler_radians.x)
	),
	quaternion_from_axis({0, 0, 1}, euler_radians.z),
);
*/
inline Quaternion quaternion_from_radians(Vector3 euler_radians) {
	auto half_radians = euler_radians / 2;
	auto s = sine(half_radians);
	auto c = cosine(half_radians);
	return {
		s.y * c.x * s.z + c.y * s.x * c.z,
		s.y * c.x * c.z - c.y * s.x * s.z,
		c.y * c.x * s.z - s.y * s.x * c.z,
		c.y * c.x * c.z + s.y * s.x * s.z
	};
};

constexpr inline Quaternion quaternion_reciprocal(Quaternion value) {
	return {-value.x, -value.y, -value.z, value.w};
}

/*
This code is a result of expanding following expression,
excluding stuff negated by multiplication with zero

return quaternion_multiply(
	quaternion_multiply(quaternion, {vector, 0}),
	quaternion_reciprocal(quaternion)
);
*/
constexpr inline Vector3 quaternion_rotate_vector(Quaternion quaternion, Vector3 vector) {
	Quaternion reciprocal = quaternion_reciprocal(quaternion);
	
	Vector3 product_axis = cross_product(quaternion.xyz, vector) + vector * quaternion.w;
	return cross_product(product_axis, reciprocal.xyz)
		+ product_axis * reciprocal.w
		- reciprocal.xyz * dot_product(quaternion.xyz, vector);
}

/*
This code is a result of expanding following expressions,
excluding stuff negated by multiplication with zero

right   = quaternion_rotate_vector(quaternion, {1, 0, 0});
up      = quaternion_rotate_vector(quaternion, {0, 1, 0});
forward = quaternion_rotate_vector(quaternion, {0, 0, 1});
*/
inline void quaternion_get_axes(Quaternion quaternion, Vector3 & right, Vector3 & up, Vector3 & forward) {
	Quaternion reciprocal = quaternion_reciprocal(quaternion);

	Vector3 product_axis_a = {quaternion.w, quaternion.z, -quaternion.y};
	right = cross_product(product_axis_a, reciprocal.xyz)
		+ product_axis_a * reciprocal.w
		- reciprocal.xyz * quaternion.x;

	Vector3 product_axis_b = {-quaternion.z, quaternion.w, quaternion.x};
	up = cross_product(product_axis_b, reciprocal.xyz)
		+ product_axis_b * reciprocal.w
		- reciprocal.xyz * quaternion.y;

	Vector3 product_axis_c = {quaternion.y, -quaternion.x, quaternion.w};
	forward = cross_product(product_axis_c, reciprocal.xyz)
		+ product_axis_c * reciprocal.w
		- reciprocal.xyz * quaternion.z;
};

constexpr inline Vector3 quaternion_get_right(Quaternion quaternion) {
	Quaternion reciprocal = quaternion_reciprocal(quaternion);

	Vector3 product_axis_a = {quaternion.w, quaternion.z, -quaternion.y};
	return cross_product(product_axis_a, reciprocal.xyz)
		+ product_axis_a * reciprocal.w
		- reciprocal.xyz * quaternion.x;
};

constexpr inline Vector3 quaternion_get_up(Quaternion quaternion) {
	Quaternion reciprocal = quaternion_reciprocal(quaternion);

	Vector3 product_axis_b = {-quaternion.z, quaternion.w, quaternion.x};
	return cross_product(product_axis_b, reciprocal.xyz)
		+ product_axis_b * reciprocal.w
		- reciprocal.xyz * quaternion.y;
};

constexpr inline Vector3 quaternion_get_forward(Quaternion quaternion) {
	Quaternion reciprocal = quaternion_reciprocal(quaternion);

	Vector3 product_axis_c = {quaternion.y, -quaternion.x, quaternion.w};
	return cross_product(product_axis_c, reciprocal.xyz)
		+ product_axis_c * reciprocal.w
		- reciprocal.xyz * quaternion.z;
};

//
// Macro implementations
//

#define CLAMP_IMPL(T)\
constexpr inline T clamp(T value, T low, T high) {\
	return min(max(value, low), high);\
}

#define MOVE_TOWARDS_CLAMPED_IMPL(T)\
constexpr inline T move_towards_clamped(T from, T to, T delta) {\
	return clamp(from + delta, from, to);\
}

#define VECTOR_MAGNITUDE_SQUARED_IMPL(T, S)\
constexpr inline S magnitude_squared(T value) {\
	return dot_product(value, value);\
}

#define VECTOR_MAGNITUDE_IMPL(T)\
inline float magnitude(T value) {\
	return square_root(magnitude_squared(value));\
}

#define VECTOR_NORMALIZE_IMPL(T)\
inline T normalize(T value) {\
	return value / magnitude(value);\
}

#define VECTOR_REFLECT_IMPL(T)\
constexpr inline T reflect(T incident, T normal, float factor) {\
	float incident_cosine = dot_product(incident, normal);\
	float normal_factor   = incident_cosine * factor;\
	return incident - normal * normal_factor;\
}

#define VECTOR_REFRACT_IMPL(T)\
inline T refract(T incident, T normal, float factor) {\
	float incident_cosine = dot_product(incident, normal);\
	float incident_sine_squared = 1 - incident_cosine * incident_cosine;\
	float refracted_sine_squared = factor * factor * incident_sine_squared;\
	if (refracted_sine_squared < 1) {\
		float refracted_cosine = square_root(1 - refracted_sine_squared);\
		float normal_factor = incident_cosine * factor + refracted_cosine;\
		return incident * factor - normal * normal_factor;\
	}\
	return {};\
}

#define VECTOR_MOVE_TOWARDS_CLAMPED_IMPL(T)\
inline T move_towards_clamped(T from, T to, float delta) {\
	T direction = to - from;\
	float distance = magnitude(direction);\
	if (delta <= 0) { return from; }\
	if (delta >= distance) { return to; }\
	return from + (direction / distance) * delta;\
}

#define VECTOR_CLAMP_MAGNITUDE_IMPL(T)\
inline T clamp_magnitude(T value, float low, float high) {\
	float length = magnitude(value);\
	if (length < low) { return (value / length) * low; }\
	if (length > high) { return (value / length) * high; }\
	return value;\
}

CLAMP_IMPL(int32)
CLAMP_IMPL(Vector2i)
CLAMP_IMPL(Vector3i)

CLAMP_IMPL(float)
CLAMP_IMPL(Vector2)
CLAMP_IMPL(Vector3)
CLAMP_IMPL(Vector4)

MOVE_TOWARDS_CLAMPED_IMPL(int32);
MOVE_TOWARDS_CLAMPED_IMPL(float);

VECTOR_MAGNITUDE_SQUARED_IMPL(Vector2i, int32)
VECTOR_MAGNITUDE_SQUARED_IMPL(Vector3i, int32)

VECTOR_MAGNITUDE_SQUARED_IMPL(Vector2, float)
VECTOR_MAGNITUDE_SQUARED_IMPL(Vector3, float)
VECTOR_MAGNITUDE_SQUARED_IMPL(Vector4, float)

VECTOR_MAGNITUDE_IMPL(Vector2)
VECTOR_MAGNITUDE_IMPL(Vector3)
VECTOR_MAGNITUDE_IMPL(Vector4)

VECTOR_NORMALIZE_IMPL(Vector2)
VECTOR_NORMALIZE_IMPL(Vector3)
VECTOR_NORMALIZE_IMPL(Vector4)

VECTOR_REFLECT_IMPL(Vector2)
VECTOR_REFLECT_IMPL(Vector3)

VECTOR_REFRACT_IMPL(Vector2)
VECTOR_REFRACT_IMPL(Vector3)

VECTOR_MOVE_TOWARDS_CLAMPED_IMPL(Vector2)
VECTOR_MOVE_TOWARDS_CLAMPED_IMPL(Vector3)
VECTOR_MOVE_TOWARDS_CLAMPED_IMPL(Vector4)

VECTOR_CLAMP_MAGNITUDE_IMPL(Vector2)
VECTOR_CLAMP_MAGNITUDE_IMPL(Vector3)
VECTOR_CLAMP_MAGNITUDE_IMPL(Vector4)

#undef CLAMP_IMPL
#undef MOVE_TOWARDS_CLAMPED_IMPL
#undef VECTOR_MAGNITUDE_SQUARED_IMPL
#undef VECTOR_MAGNITUDE_IMPL
#undef VECTOR_NORMALIZE_IMPL
#undef VECTOR_REFLECT_IMPL
#undef VECTOR_REFRACT_IMPL
#undef VECTOR_MOVE_TOWARDS_CLAMPED_IMPL
#undef VECTOR_CLAMP_MAGNITUDE_IMPL
