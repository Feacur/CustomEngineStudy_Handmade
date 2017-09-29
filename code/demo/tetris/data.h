GLOBAL_CONST int32 FIELD_WIDTH  = 11;
GLOBAL_CONST int32 FIELD_HEIGHT = 17;

GLOBAL_CONST float TIME_STEP_SIDE        = 0.1f;
GLOBAL_CONST float TIME_STEP_DOWN_NORMAL = 0.2f;
GLOBAL_CONST float TIME_STEP_DOWN_FAST   = 0.05f;

GLOBAL_CONST Complex ROTATION_VECTOR      = {1, 0};
GLOBAL_CONST Vector2 CELL_SIZE_POSITION   = {10, 10};
GLOBAL_CONST Vector2 CELL_SIZE_BACKGROUND = {10, 10};
GLOBAL_CONST Vector2 CELL_SIZE_FIGURE     = {8, 8};

GLOBAL_CONST Complexi FIGURE_ROTATION_VECTOR = {0, 1};

GLOBAL_CONST Vector4 color_background_odd  = {0.25f, 0.25f, 0.25f, 1};
GLOBAL_CONST Vector4 color_background_even = {0.1f, 0.1f, 0.1f, 1};
GLOBAL_CONST Vector4 color_field           = {0.8f, 0.8f, 0.8f, 1};
GLOBAL_CONST Vector4 color_figure          = {0.95f, 0.95f, 0.95f, 1};

struct Game_Data {
	bool is_initialized;
	bool field[FIELD_WIDTH * FIELD_HEIGHT];
	bool has_figure;
	Vector2i figure[4];
	Vector2i position;
	float elapsed_time_side;
	float elapsed_time_down;
};

GLOBAL_CONST int32 FIELD_SIZE  = C_ARRAY_LENGTH(Game_Data::field);
GLOBAL_CONST int32 FIGURE_SIZE = C_ARRAY_LENGTH(Game_Data::figure);

GLOBAL_CONST int32 FIELD_SIZE_BYTES  = sizeof(Game_Data::field);
GLOBAL_CONST int32 FIELD_WIDTH_BYTES = FIELD_SIZE_BYTES / FIELD_HEIGHT;
GLOBAL_CONST int32 FIGURE_SIZE_BYTES = sizeof(Game_Data::figure);

//
// figures
//

GLOBAL_CONST Vector2i figures[][4] {
	{{ 0,  2}, {0, 1}, {0, 0}, { 1, 0}}, // L
	{{ 0,  2}, {0, 1}, {0, 0}, {-1, 0}}, // L inverse
	{{-1,  0}, {0, 0}, {0, 1}, { 1, 1}}, // S
	{{-1,  1}, {0, 1}, {0, 0}, { 1, 0}}, // S inverse
	{{ 0, -1}, {0, 0}, {0, 1}, { 0, 2}}, // I
	{{ 0,  0}, {0, 1}, {1, 0}, { 1, 1}}, // O
	{{-1,  0}, {0, 0}, {1, 0}, { 0, 1}}, // T
};

GLOBAL_CONST int32 FIGURES_NUMBER = C_ARRAY_LENGTH(figures);

//
// funcitons
//

Game_Data *get_game_data(Platform_Data *platform_data) {
	// the "header" of platform data is game data
	auto game_data = (Game_Data *)platform_data->permanent_memory.data;
	if (!game_data->is_initialized) {
		reset_memory_chunk(&platform_data->permanent_memory);
		ALLOCATE_STRUCT(&platform_data->permanent_memory, Game_Data);
		random_set_seed((uint32)time(NULL));
		game_data->is_initialized = true;
	}
	return game_data;
}

inline void imprint_figure_into_field(Game_Data *game_data) {
	for (int32 i = 0; i < FIGURE_SIZE; ++i) {
		Vector2i p = game_data->figure[i] + game_data->position;
		if (p.y < FIELD_HEIGHT) {
			game_data->field[p.y * FIELD_WIDTH + p.x] = true;
		}
	}
}

bool move_is_valid(Game_Data *game_data, const Vector2i figure[FIGURE_SIZE], Vector2i move) {
	// 1) inside the field bounds
	// 2) except the top, which is ignored
	// 3) does not overlap with other figures

	bool is_valid = true;
	
	auto position = game_data->position + move;
	
	for (int32 i = 0; (i < FIGURE_SIZE) && is_valid; ++i) {
		Vector2i p = figure[i] + position;
		if ((p.x >= 0) && (p.x < FIELD_WIDTH) && (p.y >= 0)) {
			if (p.y < FIELD_HEIGHT) {
				bool is_filled = game_data->field[p.y * FIELD_WIDTH + p.x];
				is_valid = !is_filled;
			}
		}
		else {
			is_valid = false;
		}
	}
	
	return is_valid;
}

inline void reset_field(Game_Data *game_data) {
	memset(game_data->field, 0, FIELD_SIZE_BYTES);
	// or as a plain code
	// for (int32 i = 0; i < FIELD_SIZE; ++i) {
	// 	game_data->field[i] = 0;
	// }
}

inline bool set_figure(Game_Data *game_data, const Vector2i figure[FIGURE_SIZE]) {
	if (move_is_valid(game_data, figure, {0, 0})) {
		memcpy(game_data->figure, figure, FIGURE_SIZE_BYTES);
		// or as a plain code
		// for (int32 i = 0; i < FIGURE_SIZE; ++i) {
		// 	game_data->figure[i] = figure[i];
		// }
		return true;
	}
	return false;
}

inline void rotate_the_figure(Game_Data *game_data) {
	Vector2i figure[FIGURE_SIZE] = {};
	for (int32 i = 0; i < FIGURE_SIZE; ++i) {
		figure[i] = complex_multiply(
			game_data->figure[i], FIGURE_ROTATION_VECTOR
		);
	}

	set_figure(game_data, figure);
}

inline bool get_line_is_filled(Game_Data *game_data, int32 y) {
	bool is_filled = true;
	for (int32 x = 0; (x < FIELD_WIDTH) && is_filled; ++x) {
		is_filled = game_data->field[y * FIELD_WIDTH + x];
	}
	return is_filled;
}

inline bool get_line_is_empty(Game_Data *game_data, int32 y) {
	bool is_empty = true;
	for (int32 x = 0; (x < FIELD_WIDTH) && is_empty; ++x) {
		is_empty = !game_data->field[y * FIELD_WIDTH + x];
	}
	return is_empty;
}

inline void move_line(Game_Data *game_data, int32 from, int32 to) {
	auto source      = game_data->field + from * FIELD_WIDTH;
	auto destination = game_data->field + to * FIELD_WIDTH;
	memcpy(destination, source, FIELD_WIDTH_BYTES);
	memset(source, 0, FIELD_WIDTH_BYTES);
	// or as a plain code
	// for (int32 x = 0; x < FIELD_WIDTH; ++x) {
	// 	game_data->field[to * FIELD_WIDTH + x]   = game_data->field[from * FIELD_WIDTH + x];
	// }
	//
	// for (int32 x = 0; x < FIELD_WIDTH; ++x) {
	// 	game_data->field[from * FIELD_WIDTH + x] = false;
	// }
}

inline void collapse_field(Game_Data *game_data) {
	int32 line_to_replace = FIELD_HEIGHT;

	for (int32 y = 0; y < FIELD_HEIGHT; ++y) {
		if (get_line_is_filled(game_data, y)) {
			line_to_replace = y;
			break;
		}
	}

	for (int32 y = line_to_replace + 1; y < FIELD_HEIGHT; ++y) {
		if (!get_line_is_filled(game_data, y)) {
			move_line(game_data, y, line_to_replace);
			line_to_replace = line_to_replace + 1;
		}
	}
}
