// http://jamie-wong.com/2016/07/15/ray-marching-signed-distance-functions/
// http://iquilezles.org/www/articles/distfunctions/distfunctions.htm
// http://www.iquilezles.org/www/articles/rmshadows/rmshadows.htm
// http://www.iquilezles.org/www/articles/smin/smin.htm

#include "../../shared/software_renderer_simd.h"

#include "sdf_shapes.h"
#include "sdf_operations.h"

#include "scene_1.h"
#include "scene_common_sdf.h"
#include "scene_common_raymarch.h"
inline Vector3 apply_light(Vector3 albedo, Ray3 ray, Vector3 point, Vector3 normal, float specular_power) {
	PERSISTENT_LOCAL_CONST Vector3 ambient = {0.1f, 0.1f, 0.1f};
	PERSISTENT_LOCAL_CONST Vector3 light = normalize(vector_init(1.0f, 2.0f, 1.0f));
	PERSISTENT_LOCAL_CONST Vector3 light_color = {1, 1, 1};
	PERSISTENT_LOCAL_CONST float depth_limit = 1000;

	auto result = scale_multiply(albedo, ambient);
	
    float attenuation = raymarch_attenuation(point, normal, light, 16, depth_limit);

	float diffuse_lighting = max(0.0f, dot_product(normal, light)) * attenuation;
	result = result + scale_multiply(albedo, light_color) * diffuse_lighting;
	
	if (specular_power > 0) {
		auto reflection = reflect(ray.direction, normal, 2.0f);
		float specular_lighting_base = max(0.0f, dot_product(reflection, light));
		float specular_lighting = powf(specular_lighting_base, specular_power) * attenuation;
		result = result + light_color * specular_lighting;
	}

	return min(result, {1, 1, 1});
}

inline Vector4 raymarch_color(Ray3 ray) {
	PERSISTENT_LOCAL_CONST float depth_limit = 1000;
	Raymarch_Result raymarch_result = raymarch_scene(ray, 256, depth_limit);

	Vector4 color = {0, 0, 0, 0};
	if (raymarch_result.depth < depth_limit) {
		Vector3 point = ray.origin + ray.direction * raymarch_result.depth;
		Vector3 normal = sdf_normal(point);
        Vector3 reflection = reflect(ray.direction, normal, 2.0f);

		if (normal.y > 0.99f && point.y < 0.2f) {
			float pattern = fmodf(floorf(point.x * 0.2f) + floorf(point.z * 0.2f), 2.0f);
			float brightness = (0.3f + pattern * 0.1f);
			color.xyz = {brightness, brightness, brightness};	
		}
		else {
			color.xyz = {1, 0, 0};
		}

		color.xyz = apply_light(color.xyz, ray, point, normal, 100);
		
		color.w = 1;
	}

	return color;
}

GLOBAL_VARIABLE float rotation_radians = 0;
GAME_UPDATE(game_update) {
	platform_data->size_target = {320, 200};

	rotation_radians += 0.25f * pi * platform_data->time.delta;
	if (rotation_radians > tau) {
		rotation_radians -= tau;
	}

	PERSISTENT_LOCAL_VARIABLE float fraction_direction = 1;
	fraction += fraction_direction * platform_data->time.delta;
	if (fraction < 0) {
		fraction = 0;
		fraction_direction = 1;
	}
	else if (fraction > 1) {
		fraction = 1;
		fraction_direction = -1;
	}
}

GAME_RENDER(game_render) {
	auto image = platform_data->render_buffer_image;
	clear_buffer(image, {0, 0, 0, 0});

	Ray3 ray = {};

	Quaternion origin_rotation = quaternion_from_radians({0, rotation_radians, 0});
	ray.origin = quaternion_rotate_vector(origin_rotation, {0, 20, -25});
	
	Quaternion direction_rotation = quaternion_from_radians({pi/6, rotation_radians, 0});
	
	for (int32 y = 0; y < image.size.y; y++) {
		uint32 *destination_line = image.data + y * image.size.x;
		float offset_y = (float)(y - image.size.y / 2) / image.size.y;
		for (int32 x = 0; x < image.size.x; x++) {
			uint32 *destination_uint32 = destination_line + x;
			float offset_x = (float)(x - image.size.x / 2) / image.size.y;
			
			Vector3 screen_point = {offset_x, offset_y, 1};
			ray.direction = quaternion_rotate_vector(
				direction_rotation,
				screen_point / magnitude(screen_point)
			);

			Vector4 color = raymarch_color(ray);
			*destination_uint32 = vector4_to_color32(color, image.offsets);
		}
	}
}

GAME_OUTPUT_SOUND(game_output_sound) {
}
