GLOBAL_CONST int32 trace_depth_limit = 10;
GLOBAL_CONST int32 samples_count = 4;

GLOBAL_CONST float air_refractive_index = 1;

GLOBAL_CONST Vector4 spheres[] = {
	// floor
	{0, -100, 0, 100},
	// back
	{2, 0.5f, 2.0f, 0.5f},
	{0, 0.5f, 2.0f, 0.5f},
	{-2, 0.5f, 2.0f, 0.5f},
	// front
	{2, 0.5f, -0.5f, 0.5f},
	{0, 0.5f, -0.5f, 0.5f},
	{-2, 0.5f, -0.5f, 0.5f},
	// transparent
	{0.5f, 1.5f, -0.5f, 0.5f},
	// light	
	{-1.5f, 2.0f, 0.5f, 0.3f},
};
GLOBAL_CONST int32 spheres_count = C_ARRAY_LENGTH(spheres);

GLOBAL_CONST Material materials[] = {
	// floor
	{Material::Type::Diffuse,    {0.8f, 0.8f, 0.8f}, {0, 0, 0}, 1,    0},
	// back
	{Material::Type::Diffuse,    {0.8f, 0.4f, 0.4f}, {0, 0, 0}, 1,    0},
	{Material::Type::Diffuse,    {0.4f, 0.8f, 0.4f}, {0, 0, 0}, 1,    0},
	{Material::Type::Metal,      {0.4f, 0.4f, 0.8f}, {0, 0, 0}, 0,    0},
	// front
	{Material::Type::Metal,      {0.4f, 0.8f, 0.4f}, {0, 0, 0}, 0,    0},
	{Material::Type::Metal,      {0.4f, 0.8f, 0.4f}, {0, 0, 0}, 0.2f, 0},
	{Material::Type::Metal,      {0.4f, 0.8f, 0.4f}, {0, 0, 0}, 0.6f, 0},
	// transparent
	{Material::Type::Dielectric, {1.0f, 1.0f, 1.0f}, {0, 0, 0}, 0,    1.7f},
	// light
	{Material::Type::Diffuse,    {0.0f, 0.0f, 0.0f}, {2, 2, 2}, 0,    0},
};

void reinit_game_data(Game_Data *game_data) {
	game_data->is_initialized = true;
	game_data->camera_position = {0.0f, 2.5f, -3.0f};
	game_data->camera_rotation = quaternion_from_radians(
		{deg_to_rad(33.69), deg_to_rad(0), deg_to_rad(0)}
	);
}
