#include <math.h>
#include <intrin.h>

GLOBAL_CONST float epsilon = 0.00001f;
GLOBAL_CONST float pi      = 3.14159265359f;
GLOBAL_CONST float tau     = pi * 2;

//
// Floating point math (32 bit)
//

constexpr inline float sign(float value) {
	return (value < 0.0f) ? -1.0f : 1.0f;
}

#undef min
constexpr inline float min(float first, float second) {
	return (first < second) ? first : second;
}

#undef max
constexpr inline float max(float first, float second) {
	return (first > second) ? first : second;
}

constexpr inline float clamp(float value, float low, float high) {
	return min(max(value, low), high);
}

constexpr inline float move_towards(float from, float to, float delta) {
	return clamp(from + delta, from, to);
}

constexpr inline float interpolate(float from, float to, float fraction) {
	return from + (to - from) * fraction;
}

inline float absolute(float value) {
	return fabsf(value);
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

//
// Integer math (32 bit)
//

constexpr inline int32 sign(int32 value) {
	return (value < 0) ? -1 : 1;
}

constexpr inline int32 min(int32 first, int32 second) {
	return (first < second) ? first : second;
}

constexpr inline int32 max(int32 first, int32 second) {
	return (first > second) ? first : second;
}

constexpr inline int32 clamp(int32 value, int32 low, int32 high) {
	return min(max(value, low), high);
}

constexpr inline int32 move_towards(int32 from, int32 to, int32 delta) {
	return clamp(from + delta, from, to);
}

constexpr inline int32 interpolate(int32 from, int32 to, float fraction) {
	return from + (int)((to - from) * fraction);
}

inline int32 absolute(int32 value) {
	return abs(value);
}

constexpr inline int32 round_up_with_step(int32 value, int32 step) {
	int32 reminder = (value % step);
	return (reminder < 1) ? value : (value + (step - reminder));
}
