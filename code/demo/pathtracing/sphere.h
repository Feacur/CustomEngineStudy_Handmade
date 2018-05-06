inline float hit_sphere(Vector4 sphere, Ray3 ray) {
	Vector3 oc = ray.origin - sphere.xyz;
	float a = dot_product(ray.direction, ray.direction);
	float b = 2 * dot_product(oc, ray.direction);
	float c = dot_product(oc, oc) - sphere.w * sphere.w;
	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0) { return -1; }
	return (-b - square_root(discriminant)) / (2 * a);
}

inline Vector3 get_sphere_normal(Vector4 sphere, Vector3 hit) {
	return (hit - sphere.xyz) / sphere.w;
	// return normalize(hit - sphere.xyz);
}

inline Hit_Data hit_spheres(Ray3 ray, float near, float far, const Vector4 *spheres, int32 spheres_count) {
	Hit_Data result = { -1, far };
	for (int i = 0; i < spheres_count; ++i) {
		float distance_test = hit_sphere(spheres[i], ray);
		if (distance_test > near && result.distance > distance_test) {
			result = { i, distance_test };
		}
	}
	return result;
}
