struct Vector2 { float x, y; };

struct Vector3 {
	union {
		Vector2 xy;
		struct { float x, y; };
	};
	float z;
};

struct Vector4 {
	union {
		Vector3 xyz;
		struct { float x, y, z; };
	};
	float w;
};

//
// Vector2 routines
// Applications: 2d position, 2d rotation as complex number, 2d scale, 2d direction
//

constexpr inline bool operator==(Vector2 first, Vector2 second) {
	return (first.x == second.x)
		&& (first.y == second.y);
}

constexpr inline bool operator!=(Vector2 first, Vector2 second) {
	return !(first == second);
}

constexpr inline Vector2 operator+(Vector2 first, Vector2 second) {
	return {first.x + second.x, first.y + second.y};
}

constexpr inline Vector2 operator-(Vector2 first, Vector2 second) {
	return {first.x - second.x, first.y - second.y};
}

constexpr inline Vector2 operator-(Vector2 value) {
	return {-value.x, -value.y};
}

constexpr inline Vector2 operator*(Vector2 first, float second) {
	return {first.x * second, first.y * second};
}

constexpr inline Vector2 operator*(Vector2 first, Vector2 second) {
	return {first.x * second.x, first.y * second.y};
}

constexpr inline Vector2 operator/(Vector2 first, float second) {
	return {first.x / second, first.y / second};
}

constexpr inline Vector2 operator/(Vector2 first, Vector2 second) {
	return {first.x / second.x, first.y / second.y};
}

constexpr inline Vector2 vector_init(float x, float y) {
	return {x, y};
}

constexpr inline Vector2 interpolate(Vector2 from, Vector2 to, float fraction) {
	return {
		interpolate(from.x, to.x, fraction),
		interpolate(from.y, to.y, fraction)
	};
}

constexpr inline float dot_product(Vector2 first, Vector2 second) {
	return first.x * second.x
		 + first.y * second.y;
}

constexpr inline float cross_product(Vector2 first, Vector2 second) {
	return first.x * second.y - first.y * second.x;
}

constexpr inline float magnitude_squared(Vector2 value) {
	return dot_product(value, value);
}

inline float magnitude(Vector2 value) {
	return square_root(magnitude_squared(value));
}

inline Vector2 move_towards(Vector2 from, Vector2 to, float distance) {
	Vector2 direction = to - from;
	return from + direction * (distance / magnitude(direction));
}

inline Vector2 normalize(Vector2 value) {
	return value / magnitude(value);
}

constexpr inline Vector2 reflect(Vector2 incident, Vector2 normal, float factor) {
	float incident_cosine = dot_product(incident, normal);
	float normal_factor   = incident_cosine * factor;
	return incident - normal * normal_factor;
}

inline Vector2 refract(Vector2 incident, Vector2 normal, float factor) {
	float incident_cosine = dot_product(incident, normal);
	float incident_sine_squared = 1 - incident_cosine * incident_cosine;
	float refracted_sine_squared = factor * factor * incident_sine_squared;
	if (refracted_sine_squared < 1) {
		float refracted_cosine = square_root(1 - refracted_sine_squared);
		float normal_factor = incident_cosine * factor + refracted_cosine;
		return incident * factor - normal * normal_factor;
	}
	return {};
}

//
// Vector3 routines
// Applications: 3d position, 3d rotation as euler angles, 3d scale, 3d direction
//

constexpr inline bool operator==(Vector3 first, Vector3 second) {
	return (first.x == second.x)
		&& (first.y == second.y)
		&& (first.z == second.z);
}

constexpr inline bool operator!=(Vector3 first, Vector3 second) {
	return !(first == second);
}

constexpr inline Vector3 operator+(Vector3 first, Vector3 second) {
	return {first.x + second.x, first.y + second.y, first.z + second.z};
}

constexpr inline Vector3 operator-(Vector3 first, Vector3 second) {
	return {first.x - second.x, first.y - second.y, first.z - second.z};
}

constexpr inline Vector3 operator-(Vector3 value) {
	return {-value.x, -value.y, -value.z};
}

constexpr inline Vector3 operator*(Vector3 first, float second) {
	return {first.x * second, first.y * second, first.z * second};
}

constexpr inline Vector3 operator*(Vector3 first, Vector3 second) {
	return {first.x * second.x, first.y * second.y, first.z * second.z };
}

constexpr inline Vector3 operator/(Vector3 first, float second) {
	return {first.x / second, first.y / second, first.z / second};
}

constexpr inline Vector3 operator/(Vector3 first, Vector3 second) {
	return {first.x / second.x, first.y / second.y, first.z / second.z};
}

constexpr inline Vector3 vector_init(float x, float y, float z) {
	return {x, y, z};
}

constexpr inline Vector3 interpolate(Vector3 from, Vector3 to, float fraction) {
	return {
		interpolate(from.x, to.x, fraction),
		interpolate(from.y, to.y, fraction),
		interpolate(from.z, to.z, fraction)
	};
}

constexpr inline float dot_product(Vector3 first, Vector3 second) {
	return first.x * second.x
		 + first.y * second.y
		 + first.z * second.z;
}

constexpr inline Vector3 cross_product(Vector3 first, Vector3 second) {
	return {
		first.y * second.z - first.z * second.y,
		first.z * second.x - first.x * second.z,
		first.x * second.y - first.y * second.x
	};
}

constexpr inline float magnitude_squared(Vector3 value) {
	return dot_product(value, value);
}

inline float magnitude(Vector3 value) {
	return square_root(magnitude_squared(value));
}

inline Vector3 move_towards(Vector3 from, Vector3 to, float distance) {
	Vector3 direction = to - from;
	return from + direction * (distance / magnitude(direction));
}

inline Vector3 normalize(Vector3 value) {
	return value / magnitude(value);
}

constexpr inline Vector3 reflect(Vector3 incident, Vector3 normal, float factor) {
	float incident_cosine = dot_product(incident, normal);
	float normal_factor   = incident_cosine * factor;
	return incident - normal * normal_factor;
}

inline Vector3 refract(Vector3 incident, Vector3 normal, float factor) {
	float incident_cosine = dot_product(incident, normal);
	float incident_sine_squared = 1 - incident_cosine * incident_cosine;
	float refracted_sine_squared = factor * factor * incident_sine_squared;
	if (refracted_sine_squared < 1) {
		float refracted_cosine = square_root(1 - refracted_sine_squared);
		float normal_factor = incident_cosine * factor + refracted_cosine;
		return incident * factor - normal * normal_factor;
	}
	return {};
}

//
// Vector4 routines
// Applications: 3d rotation as quaternion, color
//

constexpr inline bool operator==(Vector4 first, Vector4 second) {
	return (first.x == second.x)
		&& (first.y == second.y)
		&& (first.z == second.z)
		&& (first.w == second.w);
}

constexpr inline bool operator!=(Vector4 first, Vector4 second) {
	return !(first == second);
}

constexpr inline Vector4 operator+(Vector4 first, Vector4 second) {
	return {first.x + second.x, first.y + second.y, first.z + second.z, first.w + second.w};
}

constexpr inline Vector4 operator-(Vector4 first, Vector4 second) {
	return {first.x - second.x, first.y - second.y, first.z - second.z, first.w - second.w};
}

constexpr inline Vector4 operator-(Vector4 value) {
	return {-value.x, -value.y, -value.z, -value.w};
}

constexpr inline Vector4 operator*(Vector4 first, float second) {
	return {first.x * second, first.y * second, first.z * second, first.w * second};
}

constexpr inline Vector4 operator*(Vector4 first, Vector4 second) {
	return {first.x * second.x, first.y * second.y, first.z * second.z, first.w * second.w};
}

constexpr inline Vector4 operator/(Vector4 first, float second) {
	return {first.x / second, first.y / second, first.z / second, first.w / second};
}

constexpr inline Vector4 operator/(Vector4 first, Vector4 second) {
	return {first.x / second.x, first.y / second.y, first.z / second.z, first.w / second.w};
}

constexpr inline Vector4 vector_init(float x, float y, float z, float w) {
	return {x, y, z, w};
}

constexpr inline Vector4 interpolate(Vector4 from, Vector4 to, float fraction) {
	return {
		interpolate(from.x, to.x, fraction),
		interpolate(from.y, to.y, fraction),
		interpolate(from.z, to.z, fraction),
		interpolate(from.w, to.w, fraction)
	};
}

constexpr inline float dot_product(Vector4 first, Vector4 second) {
	return first.x * second.x
		 + first.y * second.y
		 + first.z * second.z
		 + first.w * second.w;
}

constexpr inline float magnitude_squared(Vector4 value) {
	return dot_product(value, value);
}

inline float magnitude(Vector4 value) {
	return square_root(magnitude_squared(value));
}

inline Vector4 move_towards(Vector4 from, Vector4 to, float distance) {
	Vector4 direction = to - from;
	return from + direction * (distance / magnitude(direction));
}

inline Vector4 normalize(Vector4 value) {
	return value / magnitude(value);
}

constexpr inline Vector4 reflect(Vector4 incident, Vector4 normal, float factor) {
	float incident_cosine = dot_product(incident, normal);
	float normal_factor   = incident_cosine * factor;
	return incident - normal * normal_factor;
}

inline Vector4 refract(Vector4 incident, Vector4 normal, float factor) {
	float incident_cosine = dot_product(incident, normal);
	float incident_sine_squared = 1 - incident_cosine * incident_cosine;
	float refracted_sine_squared = factor * factor * incident_sine_squared;
	if (refracted_sine_squared < 1) {
		float refracted_cosine = square_root(1 - refracted_sine_squared);
		float normal_factor = incident_cosine * factor + refracted_cosine;
		return incident * factor - normal * normal_factor;
	}
	return {};
}
