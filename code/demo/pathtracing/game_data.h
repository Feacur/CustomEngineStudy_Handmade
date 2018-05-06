struct Game_Data {
	bool is_initialized;
	Vector3 camera_position;
	Quaternion camera_rotation;
};

struct Hit_Data {
	int32 index;
	float distance;
};

struct Material {
	enum class Type {Diffuse, Metal, Dielectric};
	Type type;
	Vector3 albedo;
	Vector3 emission;
	float roughness;
	float refractive_index;
};

//
// funcitons
//

void reinit_game_data(Game_Data *game_data);

Game_Data *get_game_data() {
	Game_Data *game_data;
	if (permanent_memory->used == 0) {
		reset_memory_chunk(permanent_memory);
		game_data = ALLOCATE_STRUCT(permanent_memory, Game_Data);
		reinit_game_data(game_data);
	}
	else {
		// the "header" of platform data is game data
		game_data = (Game_Data *)permanent_memory->data;
	}
	return game_data;
}
