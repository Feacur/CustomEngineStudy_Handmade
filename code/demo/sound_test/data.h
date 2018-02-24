GLOBAL_CONST Complex ROTATION_VECTOR = {1, 0};

struct Game_Data {
	bool is_initialized;
};

//
// funcitons
//

Game_Data *get_game_data(Platform_Data *platform_data) {
	// the "header" of platform data is game data
	auto game_data = (Game_Data *)platform_data->permanent_memory.data;
	if (!game_data->is_initialized) {
		reset_memory_chunk(&platform_data->permanent_memory);
		ALLOCATE_STRUCT(&platform_data->permanent_memory, Game_Data);
		game_data->is_initialized = true;
	}
	return game_data;
}
