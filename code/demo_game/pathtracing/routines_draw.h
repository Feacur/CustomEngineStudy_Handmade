void draw_hit_shapes(Game_Data * game_data, Vector3 camera_position, Quaternion camera_rotation) {
	Vector2i size = globals::render_buffer_f.size;
	Vector2i half_size = size / 2;

	float fov_x = 0.5f * pi;
	float z = half_size.x * tangent(fov_x * 0.5f);

	for (int32 y = 0; y < size.y; ++y) {
		Vector4 * destination_line = globals::render_buffer_f.data + y * size.x;
		// float offset_y = (float)(y - half_size.y) / size.y;
		float offset_y = (float)(y - half_size.y);
		for (int32 x = 0; x < size.x; ++x) {
			Vector4 * destination = destination_line + x;
			// float offset_x = (float)(x - half_size.x) / size.y;
			float offset_x = (float)(x - half_size.x);
			
			// Vector3 camera_direction = {offset_x, offset_y, 1};

			Ray3 camera_ray = camera_sample_ray(
				game_data,
				camera_position, camera_rotation,
				{offset_x, offset_y, z},
				camera_focus_distance, camera_aperture
			);
			Vector3 color = scene_sample_color(game_data, camera_ray, trace_depth_limit);
			*destination = {destination->xyz + color, 1};
		}
	}
	globals::render_buffer_f.exposure += 1;
}
