/*
These functions mimic those that are used in shader code.
*/

//
// Vector2 routines
//

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

inline Vector2 absolute(Vector2 value) {
	return {
		absolute(value.x),
		absolute(value.y)
	};
}

//
// Vector3 routines
//

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

inline Vector3 absolute(Vector3 value) {
	return {
		absolute(value.x),
		absolute(value.y),
		absolute(value.z)
	};
}

//
// Vector4 routines
//

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

inline Vector4 absolute(Vector4 value) {
	return {
		absolute(value.x),
		absolute(value.y),
		absolute(value.z),
		absolute(value.w)
	};
}
