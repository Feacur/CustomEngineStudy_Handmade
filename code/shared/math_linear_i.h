struct Vector2i { int32 x, y; };

struct Vector3i {
	union {
		Vector2i xy;
		struct { int32 x, y; };
	};
	int32 z;
};

//
// Vector2i routines
// Applications: 2d position
//

constexpr inline bool operator==(Vector2i first, Vector2i second) {
	return (first.x == second.x)
		&& (first.y == second.y);
}

constexpr inline bool operator!=(Vector2i first, Vector2i second) {
	return !(first == second);
}

constexpr inline Vector2i operator+(Vector2i first, Vector2i second) {
	return {first.x + second.x, first.y + second.y};
}

constexpr inline Vector2i operator-(Vector2i first, Vector2i second) {
	return {first.x - second.x, first.y - second.y};
}

constexpr inline Vector2i operator-(Vector2i value) {
	return {-value.x, -value.y};
}

constexpr inline Vector2i operator*(Vector2i first, int32 second) {
	return {first.x * second, first.y * second};
}

constexpr inline Vector2i operator*(Vector2i first, Vector2i second) {
	return {first.x * second.x, first.y * second.y};
}

constexpr inline Vector2i operator/(Vector2i first, int32 second) {
	return {first.x / second, first.y / second};
}

constexpr inline Vector2i operator/(Vector2i first, Vector2i second) {
	return {first.x / second.x, first.y / second.y};
}

constexpr inline Vector2i vector_init(int32 x, int32 y) {
	return {x, y};
}

constexpr inline int32 dot_product(Vector2i first, Vector2i second) {
	return first.x * second.x
		 + first.y * second.y;
}

constexpr inline int32 cross_product(Vector2i first, Vector2i second) {
	return first.x * second.y - first.y * second.x;
}

constexpr inline int32 magnitude_squared(Vector2i value) {
	return dot_product(value, value);
}


//
// Vector3i routines
// Applications: 3d position
//

constexpr inline bool operator==(Vector3i first, Vector3i second) {
	return (first.x == second.x)
		&& (first.y == second.y)
		&& (first.z == second.z);
}

constexpr inline bool operator!=(Vector3i first, Vector3i second) {
	return !(first == second);
}

constexpr inline Vector3i operator+(Vector3i first, Vector3i second) {
	return {first.x + second.x, first.y + second.y, first.z + second.z};
}

constexpr inline Vector3i operator-(Vector3i first, Vector3i second) {
	return {first.x - second.x, first.y - second.y, first.z - second.z};
}

constexpr inline Vector3i operator-(Vector3i value) {
	return {-value.x, -value.y, -value.z};
}

constexpr inline Vector3i operator*(Vector3i first, int32 second) {
	return {first.x * second, first.y * second, first.z * second};
}

constexpr inline Vector3i operator*(Vector3i first, Vector3i second) {
	return {first.x * second.x, first.y * second.y, first.z * second.z};
}

constexpr inline Vector3i operator/(Vector3i first, int32 second) {
	return {first.x / second, first.y / second, first.z / second};
}

constexpr inline Vector3i operator/(Vector3i first, Vector3i second) {
	return {first.x / second.x, first.y / second.y, first.z / second.z};
}

constexpr inline Vector3i vector_init(int32 x, int32 y, int32 z) {
	return {x, y, z};
}

constexpr inline int32 dot_product(Vector3i first, Vector3i second) {
	return first.x * second.x
		 + first.y * second.y
		 + first.z * second.z;
}

constexpr inline Vector3i cross_product(Vector3i first, Vector3i second) {
	return {
		first.y * second.z - first.z * second.y,
		first.z * second.x - first.x * second.z,
		first.x * second.y - first.y * second.x
	};
}

constexpr inline int32 magnitude_squared(Vector3i value) {
	return dot_product(value, value);
}
