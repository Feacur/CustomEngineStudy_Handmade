GLOBAL_CONST Vector3 vector3_zero = {0, 0, 0};

inline Vector3 random_in_unit_radius() {
	Vector3 result;
	do {
		result = {random_01() * 2 - 1, random_01() * 2 - 1, random_01() * 2 - 1};
	} while (magnitude_squared(result) > 1);
	return result;
}

inline Vector3 get_sky_color(Ray3 ray) {
	PERSISTENT_LOCAL_CONST Vector3 sky_color_1 = {1, 1, 1};
	PERSISTENT_LOCAL_CONST Vector3 sky_color_2 = {0.5f, 0.7f, 1};
	float fraction = (ray.direction.y + 1) / 2;
	return interpolate(sky_color_1, sky_color_2, fraction);
}

inline float schlick(float cosine, float factor) {
	float r = (factor - 1) / (factor + 1);
	return interpolate(powf(1 - cosine, 5), 1.0f, r * r);
}

inline Vector3 scatter_ray(Material material, Vector3 incident, Vector3 normal) {
	PERSISTENT_LOCAL_CONST float factor = 2;
	
	normal = normalize(normal + random_in_unit_radius() * material.roughness);

	if (material.type == Material::Type::Metal) {
		return reflect(incident, normal, factor);
	}
	if (material.type == Material::Type::Dielectric) {
		float refraction_factor;
		float cosine = dot_product(incident, normal);
		if (cosine < 0) {
			refraction_factor = air_refractive_index / material.refractive_index;
			cosine = -cosine;
		}
		else {
			refraction_factor = material.refractive_index / air_refractive_index;
			normal = -normal;
		}

		auto scattered = refract(incident, normal, refraction_factor);
		if (scattered == vector3_zero || random_01() < schlick(cosine, refraction_factor)) {
			scattered = reflect(incident, normal, factor);
		}
		return scattered;
	}
	return normal;
}

inline Vector3 get_scene_color(Ray3 ray, int depth) {
	Vector3 color = {};
	Vector3 attenuation = {1, 1, 1};
	for (int i = 0; i < depth; ++i) {
		auto data = hit_spheres(ray, 0.001f, INFINITY, spheres, spheres_count);
		if (data.index >= 0) {
			auto sphere = spheres[data.index];
			auto material = materials[data.index];
			
			auto hit = ray_point(ray, data.distance);
			auto normal = get_sphere_normal(sphere, hit);
			ray = {hit, scatter_ray(material, ray.direction, normal)};
			
			color = color + (attenuation * material.emission);
			attenuation = (attenuation * material.albedo);
		}
		else {
			color = color + (attenuation * get_sky_color(ray));
			// attenuation = {};
			break;
		}
	}
	return color;
}

inline Vector3 multisample_scene_color(Vector3 camera_position, Quaternion camera_rotation, float x, float y, float size, int32 iterations) {
	Vector3 color = {};
	for (int i = 0; i < iterations; ++i) {
		Vector3 camera_direction = {
			(x + random_01()) / size,
			(y + random_01()) / size,
			1
		};
		camera_direction = normalize(camera_direction);
		camera_direction = quaternion_rotate_vector(camera_rotation, camera_direction);
		Ray3 ray = {camera_position, camera_direction};

		color = color + get_scene_color(ray, trace_depth_limit);
	}
	return color / (float)iterations;
}

void draw_hit_spheres(Vector3 camera_position, Quaternion camera_rotation) {
	// PERSISTENT_LOCAL_VAR float frame_maximum = 1.0f;
	// float next_maximum = frame_maximum;
	auto size = image.size;
	auto half_size = size / 2;
	for (int y = 0; y < size.y; ++y) {
		uint32 *destination_line = image.data + y * size.x;
		// float offset_y = (float)(y - half_size.y) / size.y;
		float offset_y = (float)(y - half_size.y);
		for (int x = 0; x < size.x; ++x) {
			uint32 *destination_uint32 = destination_line + x;
			// float offset_x = (float)(x - half_size.x) / size.y;
			float offset_x = (float)(x - half_size.x);
			
			// Vector3 camera_direction = {offset_x, offset_y, 1};

			Vector3 color = multisample_scene_color(
				camera_position, camera_rotation,
				offset_x, offset_y, (float)size.y,
				samples_count
			);
			// next_maximum = max(next_maximum, max(color));
			// color = color / frame_maximum;
			color = clamp(color, {0, 0, 0}, {1, 1, 1});

			// color = {square_root(color.x), square_root(color.y), square_root(color.z)};
			*destination_uint32 = vector4_to_color32({color, 1}, image.offsets);
		}
	}
	// frame_maximum = next_maximum;
}

void draw_gradient_2d_red_green() {
	auto size = image.size;
	for (int32 y = 0; y < size.y; ++y) {
		uint32 *destination_line = image.data + y * size.x;
		float color_g = (float)y / (size.y - 1);
		for (int32 x = 0; x < size.x; ++x) {
			uint32 *destination_uint32 = destination_line + x;
			float color_r = (float)x / (size.x - 1);
			*destination_uint32 = vector4_to_color32({color_r, color_g, 0, 1}, image.offsets);
		}
	}
}
