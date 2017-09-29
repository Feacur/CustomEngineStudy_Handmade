GLOBAL_CONST int32 FIELD_WIDTH  = 70;
GLOBAL_CONST int32 FIELD_HEIGHT = 70;

GLOBAL_CONST float TIME_STEP = 0.1f;

GLOBAL_CONST Complex ROTATION_VECTOR      = {1, 0};
GLOBAL_CONST Vector2 CELL_SIZE_POSITION   = {10, 10};
GLOBAL_CONST Vector2 CELL_SIZE_BACKGROUND = {10, 10};
GLOBAL_CONST Vector2 CELL_SIZE_FIGURE     = {8, 8};

GLOBAL_CONST Vector4 color_background_odd  = {0.25f, 0.25f, 0.25f, 1};
GLOBAL_CONST Vector4 color_background_even = {0.1f, 0.1f, 0.1f, 1};
GLOBAL_CONST Vector4 color_field           = {0.8f, 0.8f, 0.8f, 1};
GLOBAL_CONST Vector4 color_pointer         = {0.8f, 0.4f, 0.2f, 0.5f};

struct Game_Data {
	bool is_initialized;
	bool field[FIELD_WIDTH * FIELD_HEIGHT];
	bool is_active;
	float elapsed_time;
};

GLOBAL_CONST int32 FIELD_SIZE = C_ARRAY_LENGTH(Game_Data::field);

GLOBAL_CONST int32 FIELD_SIZE_BYTES  = sizeof(Game_Data::field);
GLOBAL_CONST int32 FIELD_WIDTH_BYTES = FIELD_SIZE_BYTES / FIELD_HEIGHT;

//
// figures
//

struct figure_prefab {
	bool *data;
	Vector2i size;
};

GLOBAL_CONST bool figure_glider_gun_data[36 * 9] {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
GLOBAL_CONST figure_prefab figure_glider_gun = {(bool *)figure_glider_gun_data, {36, 9}};

GLOBAL_CONST bool figure_lwss_data[5 * 4] {
	0, 1, 1, 1, 1,
	1, 0, 0, 0, 1,
	0, 0, 0, 0, 1,
	1, 0, 0, 0, 0,
};
GLOBAL_CONST figure_prefab figure_lwss = {(bool *)figure_lwss_data, {5, 4}};

GLOBAL_CONST bool figure_glider_data[3 * 3] {
	0, 1, 0,
	0, 0, 1,
	1, 1, 1,
};
GLOBAL_CONST figure_prefab figure_glider = {(bool *)figure_glider_data, {3, 3}};

GLOBAL_CONST bool figure_pulsar_data[5 * 5] {
	1, 0, 1, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 1, 0, 1,
};
GLOBAL_CONST figure_prefab figure_pulsar = {(bool *)figure_pulsar_data, {5, 5}};

//
// funcitons
//
void reset_field(Game_Data *game_data);

Game_Data *get_game_data(Platform_Data *platform_data) {
	// the "header" of platform data is game data
	auto game_data = (Game_Data *)platform_data->permanent_memory.data;
	if (!game_data->is_initialized) {
		reset_memory_chunk(&platform_data->permanent_memory);
		ALLOCATE_STRUCT(&platform_data->permanent_memory, Game_Data);
		reset_field(game_data);
		game_data->is_initialized = true;
	}
	return game_data;
}

inline void set_field_cell(Game_Data *game_data, Vector2i cell, bool value) {
	if ((cell.x >= 0) && (cell.y >= 0) && (cell.x < FIELD_WIDTH) && (cell.y < FIELD_HEIGHT)) {
		game_data->field[cell.y * FIELD_WIDTH + cell.x] = value;
	}
}

void reset_field(Game_Data *game_data) {
	memset(game_data->field, 0, FIELD_SIZE_BYTES);
	// or as a plain code
	// for (int32 i = 0; i < FIELD_SIZE; ++i) {
	// 	game_data->field[i] = 0;
	// }
	
	// Default figure
	auto figure = figure_glider_gun;
	Vector2i point_zero = {10, 50};
	for (int32 y = 0; y < figure.size.y; ++y) {
		for (int32 x = 0; x < figure.size.x; ++x) {
			Vector2i cell = {point_zero.x + x, point_zero.y + y};
			set_field_cell(game_data, cell, figure.data[y * figure.size.x + x]);
		}
	}
}

inline bool get_cell_clamp(Game_Data *game_data, int32 x, int32 y) {
	if ((x >= 0) && (y >= 0) && (x < FIELD_WIDTH) && (y < FIELD_HEIGHT)) {
		return game_data->field[y * FIELD_WIDTH + x];
	}
	return false;
}

inline bool get_cell_wrap(Game_Data *game_data, int32 x, int32 y) {
	x = (x + FIELD_WIDTH) % FIELD_WIDTH;
	y = (y + FIELD_HEIGHT) % FIELD_HEIGHT;
	return game_data->field[y * FIELD_WIDTH + x];
}

#define GET_CELL get_cell_wrap
inline bool get_cell_will_be_alive(Game_Data *game_data, int32 x, int32 y) {
	int32 n1 = GET_CELL(game_data, x - 1, y + 1);
	int32 n2 = GET_CELL(game_data, x    , y + 1);
	int32 n3 = GET_CELL(game_data, x + 1, y + 1);
	int32 n4 = GET_CELL(game_data, x - 1, y    );
	int32 n5 = GET_CELL(game_data, x + 1, y    );
	int32 n6 = GET_CELL(game_data, x - 1, y - 1);
	int32 n7 = GET_CELL(game_data, x    , y - 1);
	int32 n8 = GET_CELL(game_data, x + 1, y - 1);

	int32 neighbours_number = n1 + n2 + n3 + n4 + n5 + n6 + n7 + n8;

	// Cell will carry its state on
	if (neighbours_number == 2) {
		return game_data->field[y * FIELD_WIDTH + x];
	}
	
	// Cell will become alive
	if (neighbours_number == 3) {
		return true;
	}
	
	// Cell will die as if by underpopulation or overpopulation
	return false;
}

void update_field(Game_Data *game_data) {
	bool field[FIELD_SIZE] = {};

	for (int32 y = 0; y < FIELD_HEIGHT; ++y) {
		for (int32 x = 0; x < FIELD_WIDTH; ++x) {
			field[y * FIELD_WIDTH + x] = get_cell_will_be_alive(game_data, x, y);
		}
	}

	memcpy(game_data->field, field, FIELD_SIZE_BYTES);
	// or as a plain code
	// for (int32 i = 0; i < FIELD_SIZE; ++i) {
	// 	game_data->field[i] = field[i];
	// }
}
