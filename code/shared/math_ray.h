struct Ray3 {
	Vector3 origin;
	Vector3 direction;
};

constexpr inline Vector3 ray_point(Ray3 ray, float distance) {
	return ray.origin + ray.direction * distance;
}
