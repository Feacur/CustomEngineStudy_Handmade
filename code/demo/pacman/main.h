#include "shared/software_renderer_simd.h"
// #include "shared/random_lehmer.h"
#include "shared/random_xorshift32.h"

#include "data.h"

void draw_input(RGBA_Data image, int32 x, int32 y, bool state)
{
	auto cell_offset_background = (CELL_SIZE_POSITION - CELL_SIZE_BACKGROUND) / 2;

	Vector4 color = state ? vector_init(1, 0, 0, 0.5f) : vector_init(1, 0, 0, 0.25f);
	
	Vector2 xy = {(float)x, (float)y};
	Vector2 position = (xy * CELL_SIZE_POSITION) + cell_offset_background;
	draw_rectangle(image, position, ROTATION_VECTOR, CELL_SIZE_BACKGROUND, color);
}

DLL_EXPORT GAME_UPDATE(game_update) {
	permanent_memory = &platform_data->permanent_memory;
	transient_memory = &platform_data->transient_memory;
	input_keyboard   = &platform_data->input_keyboard;

	auto game_data = get_game_data();
	reset_memory_chunk(transient_memory);
	
	auto FIELD_WIDTH  = game_data->size.x;
	auto FIELD_HEIGHT = game_data->size.y;

	platform_data->size_target = {
		(int32)(FIELD_WIDTH * CELL_SIZE_POSITION.x),
		(int32)(FIELD_HEIGHT * CELL_SIZE_POSITION.y)
	};
	
	// process input
	input_left  = input::get_current(Keyboard_Keys::Left);
	input_right = input::get_current(Keyboard_Keys::Right);
	input_down  = input::get_current(Keyboard_Keys::Down);
	input_up    = input::get_current(Keyboard_Keys::Up);

	Vector2i move = { input_right - input_left, input_up - input_down };

	game_data->elapsed_time += platform_data->time.delta;
	if (game_data->elapsed_time > TIME_STEP) {
		game_data->elapsed_time = 0;
		character::step_adjacent(game_data, 0, move);
		ai::update(game_data);
	}
}

DLL_EXPORT GAME_RENDER(game_render) {
	auto image = platform_data->render_buffer_image;
	clear_buffer(image, {0, 0, 0, 0});

	auto cell_offset_background = (CELL_SIZE_POSITION - CELL_SIZE_BACKGROUND) / 2;
	auto cell_offset_wall = (CELL_SIZE_POSITION - CELL_SIZE_WALL) / 2;
	auto cell_offset_character = (CELL_SIZE_POSITION - CELL_SIZE_CHARACTER) / 2;

	auto game_data = get_game_data();
	
	auto FIELD_WIDTH  = game_data->size.x;
	auto FIELD_HEIGHT = game_data->size.y;

	// draw background
	for (int32 y = 0; y < FIELD_HEIGHT; ++y) {
		for (int32 x = 0; x < FIELD_WIDTH; ++x) {
			bool isOdd = (x + y) % 2;
			Vector4 color = isOdd ? color_background_odd : color_background_even;
			Vector2 xy = {(float)x, (float)y};
			Vector2 position = (xy * CELL_SIZE_POSITION) + cell_offset_background;
			draw_rectangle(image, position, ROTATION_VECTOR, CELL_SIZE_BACKGROUND, color);
		}
	}
	
	// draw field
	for (int32 y = 0; y < FIELD_HEIGHT; ++y) {
		for (int32 x = 0; x < FIELD_WIDTH; ++x) {
			bool is_filled = game_data->field[y * FIELD_WIDTH + x];
			if (is_filled) {
				Vector2 xy = {(float)x, (float)y};
				Vector2 position = (xy * CELL_SIZE_POSITION) + cell_offset_wall;
				draw_rectangle(image, position, ROTATION_VECTOR, CELL_SIZE_WALL, color_field);
			}
		}
	}
	
	// draw characters
	for (int32 i = 0; i < CHARACTERS_COUNT; ++i) {
		auto character = game_data->characters[i];
		auto color_character = color_characters[i];
		Vector2 xy = {(float)character.position.x, (float)character.position.y};
		Vector2 position = (xy * CELL_SIZE_POSITION) + cell_offset_character;
		draw_rectangle(image, position, ROTATION_VECTOR, CELL_SIZE_CHARACTER, color_character);
	}

	// draw input
	draw_input(image, FIELD_WIDTH - 1, FIELD_HEIGHT - 2, input_right);
	draw_input(image, FIELD_WIDTH - 3, FIELD_HEIGHT - 2, input_left);
	draw_input(image, FIELD_WIDTH - 2, FIELD_HEIGHT - 1, input_up);
	draw_input(image, FIELD_WIDTH - 2, FIELD_HEIGHT - 3, input_down);
}

// DLL_EXPORT GAME_OUTPUT_SOUND(game_output_sound) {
// 	auto game_data = get_game_data();
// }
