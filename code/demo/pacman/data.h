GLOBAL_VAR Memory_Chunk *permanent_memory;
GLOBAL_VAR Memory_Chunk *transient_memory;

GLOBAL_VAR Input_Keyboard *input_keyboard;

GLOBAL_CONST float TIME_STEP = 0.1f;

GLOBAL_CONST Complex ROTATION_VECTOR      = {1, 0};
GLOBAL_CONST Vector2 CELL_SIZE_POSITION   = {10, 10};
GLOBAL_CONST Vector2 CELL_SIZE_BACKGROUND = {10, 10};
GLOBAL_CONST Vector2 CELL_SIZE_WALL       = {10, 10};
GLOBAL_CONST Vector2 CELL_SIZE_CHARACTER  = {8, 8};

GLOBAL_CONST Vector4 color_background_odd  = {0.75f, 0.75f, 0.75f, 1};
GLOBAL_CONST Vector4 color_background_even = {0.9f, 0.9f, 0.9f, 1};
GLOBAL_CONST Vector4 color_field           = {0.2f, 0.2f, 0.2f, 1};

GLOBAL_CONST Vector4 color_characters[] = {
	{0.2f, 0.6f, 0.8f, 0.5f},
	{0.6f, 0.2f, 0.4f, 0.5f},
};

GLOBAL_VAR bool input_left;
GLOBAL_VAR bool input_right;
GLOBAL_VAR bool input_down;
GLOBAL_VAR bool input_up;

struct Character {
	int32 type;
	Vector2i position;
	Vector2i move_remainder;
};

struct Game_Data {
	bool is_initialized;
	// actual data
	bool *field;
	Vector2i size;
	Character characters[2];
	// controls
	float elapsed_time;
};

GLOBAL_CONST int32 CHARACTERS_COUNT = C_ARRAY_LENGTH(Game_Data::characters);

//
// test field
//

struct field_prefab {
	bool *data;
	Vector2i size;
};

GLOBAL_CONST bool field1_data[20 * 11] {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
	1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1,
	1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
	1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1,
	1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
	1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1,
	1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};
GLOBAL_CONST field_prefab field1 = {(bool *)field1_data, {20, 11}};

//
// funcitons
//
void reinit_game_data();

namespace field {
	void reset(Game_Data *game_data);
}

namespace character {
	void init(Game_Data *game_data, int32 character_index);
	void randomize_position(Game_Data *game_data, int32 character_index);
}

Game_Data *get_game_data() {
	// the "header" of platform data is game data
	auto game_data = (Game_Data *)permanent_memory->data;
	if (!game_data->is_initialized) {
		reinit_game_data();
	}
	return game_data;
}

void reinit_game_data() {
	reset_memory_chunk(permanent_memory);
	auto game_data = ALLOCATE_STRUCT(permanent_memory, Game_Data);
	
	field::reset(game_data);
	for (int32 i = 0; i < CHARACTERS_COUNT; ++i) {
		character::init(game_data, i);
		character::randomize_position(game_data, i);
	}

	game_data->is_initialized = true;
}

namespace field {
	void reset(Game_Data *game_data) {
		ASSERT_TRUE(permanent_memory, "Global var \"permanent_memory\" wasn't initialized");
		
		auto prefab = field1;
		auto cells_count = prefab.size.x * prefab.size.y;

		game_data->field = ALLOCATE_ARRAY(permanent_memory, bool, cells_count);
		memcpy(game_data->field, prefab.data, cells_count);
		
		game_data->size  = prefab.size;
	}

	bool get_cell(Game_Data *game_data, Vector2i position) {
		auto index = position.y * game_data->size.x + position.x;
		return game_data->field[index];
	}
}
	
bool test_step(Game_Data *game_data, Vector2i position, Vector2i move) {
	return (move != vector_init(0, 0))
		&& (field::get_cell(game_data, position + move) == false);
}

namespace character {
	void init(Game_Data *game_data, int32 character_index) {
		auto character = &game_data->characters[character_index];
		character->type = character_index;
	}

	void randomize_position(Game_Data *game_data, int32 character_index) {
		auto character = &game_data->characters[character_index];
		auto cells_count = game_data->size.x * game_data->size.y;
		auto offset = random_in_range(0, cells_count);
		for (int32 i = 0; i < cells_count; ++i) {
			auto offset_i = (i + offset) % cells_count;
			if (game_data->field[offset_i] == false) {
				character->position = {offset_i % game_data->size.x, offset_i / game_data->size.x};
				break;
			}
		}
	}

	bool step(Game_Data *game_data, int32 character_index, Vector2i move, Vector2i remainder) {
		auto character = &game_data->characters[character_index];
		auto position = character->position;

		if (!test_step(game_data, position, move)) { return false; }
		
		character->position = position + move;
		character->move_remainder = remainder;
		return true;
	}

	void step_adjacent(Game_Data *game_data, int32 character_index, Vector2i move) {
		auto character = &game_data->characters[character_index];

		if (move == vector_init(0, 0)) {
			character->move_remainder = {0, 0};
			return;
		}

		Vector2i move_x = { move.x, 0 };
		Vector2i move_y = { 0, move.y };

		// prefer last turn's direction
		auto remainder = character->move_remainder;
		if ((move_x == remainder) && step(game_data, character_index, move_x, move_y)) { return; }
		if ((move_y == remainder) && step(game_data, character_index, move_y, move_x)) { return; }

		// move x or y as normally
		if (step(game_data, character_index, move_x, move_y)) { return; }
		if (step(game_data, character_index, move_y, move_x)) { return; }

		// reset last turn's direction
		character->move_remainder = {0, 0};
	}
}

namespace ai {
	void move_to(Game_Data *game_data, int32 character_index, Vector2i target) {
		auto character = &game_data->characters[character_index];
		auto direction = target - character->position;
		Vector2i move = {sign(direction.x), sign(direction.y)};
		character::step_adjacent(game_data, character_index, move);
	}

	void update_character(Game_Data *game_data, int32 character_index) {
		auto character0 = game_data->characters[0];
		auto character = &game_data->characters[character_index];
		
		move_to(game_data, character_index, character0.position);
		if (character->position == character0.position) {
			character::randomize_position(game_data, character_index);
		}
	}
	
	void update(Game_Data *game_data) {
		for (int32 i = 1; i < CHARACTERS_COUNT; ++i) {
			update_character(game_data, i);
		}
	}
}

namespace input {
	inline bool get_current(Keyboard_Keys key) {
		return keyboard_get_current_state(input_keyboard, key);
	}

	inline bool get_pressed(Keyboard_Keys key) {
		return keyboard_get_transition_pressed(input_keyboard, key);
	}

	inline bool get_released(Keyboard_Keys key) {
		return keyboard_get_transition_released(input_keyboard, key);
	}
}
