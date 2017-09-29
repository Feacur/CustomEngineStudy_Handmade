#include "../../shared/software_renderer_simd.h"
#include "../../shared/random_lehmer.h"

#include <time.h> // time
#include "data.h"

GAME_UPDATE(game_update) {
	platform_data->size_target = {
		(int32)(FIELD_WIDTH * CELL_SIZE_POSITION.x),
		(int32)(FIELD_HEIGHT * CELL_SIZE_POSITION.y)
	};

	auto game_data = get_game_data(platform_data);
	reset_memory_chunk(&platform_data->transient_memory);

	// creation
	if (!game_data->has_figure) {
		game_data->position = {FIELD_WIDTH / 2, FIELD_HEIGHT - 1};

		auto figure = figures[random_in_range(0, FIGURES_NUMBER)];
		game_data->has_figure = set_figure(game_data, figure);
		
		if (!game_data->has_figure) {
			game_data->has_figure = true;
			reset_field(game_data);
		}
	}

	// movement
	if (game_data->has_figure) {
		game_data->elapsed_time_side += platform_data->time.delta;
		game_data->elapsed_time_down += platform_data->time.delta;

		// rotate
		bool should_rotate_figure = keyboard_get_transition_pressed(&platform_data->input_keyboard, Keyboard_Keys::Up);
		if (should_rotate_figure) {
			rotate_the_figure(game_data);
		}

		// move sideways
		if (game_data->elapsed_time_side > TIME_STEP_SIDE) {
			game_data->elapsed_time_side = 0;
			
			bool should_move_left = keyboard_get_current_state(&platform_data->input_keyboard, Keyboard_Keys::Left);
			if (should_move_left && move_is_valid(game_data, game_data->figure, {-1, 0})) {
				game_data->position.x = game_data->position.x - 1;
			}
			
			bool should_move_right = keyboard_get_current_state(&platform_data->input_keyboard, Keyboard_Keys::Right);
			if (should_move_right && move_is_valid(game_data, game_data->figure, {1, 0})) {
				game_data->position.x = game_data->position.x + 1;
			}
		}

		// move down
		bool should_move_fast = keyboard_get_current_state(&platform_data->input_keyboard, Keyboard_Keys::Down);
		float time_step_down = should_move_fast ? TIME_STEP_DOWN_FAST : TIME_STEP_DOWN_NORMAL;

		if (game_data->elapsed_time_down > time_step_down) {
			game_data->elapsed_time_down = 0;
			
			if (move_is_valid(game_data, game_data->figure, {0, -1})) {
				game_data->position.y = game_data->position.y - 1;
			}
			else {
				game_data->has_figure = false;
				imprint_figure_into_field(game_data);
				collapse_field(game_data);
			}
		}
	}
}

GAME_RENDER(game_render) {
	auto image = platform_data->render_buffer_image;
	clear_buffer(image, {0, 0, 0, 0});

	auto cell_offset_background = (CELL_SIZE_POSITION - CELL_SIZE_BACKGROUND) / 2;
	auto cell_offset_figure = (CELL_SIZE_POSITION - CELL_SIZE_FIGURE) / 2;

	auto game_data = get_game_data(platform_data);

	// draw background
	for (int32 y = 0; y < FIELD_HEIGHT; ++y) {
		for (int32 x = 0; x < FIELD_WIDTH; ++x) {
			bool isOdd = (x + y) % 2;
			Vector4 color = isOdd ? color_background_odd : color_background_even;
			Vector2 position = scale_multiply({(float)x, (float)y}, CELL_SIZE_POSITION) + cell_offset_background;
			draw_rectangle(image, position, ROTATION_VECTOR, CELL_SIZE_BACKGROUND, color);
		}
	}
	
	// draw field
	for (int32 y = 0; y < FIELD_HEIGHT; ++y) {
		for (int32 x = 0; x < FIELD_WIDTH; ++x) {
			bool is_filled = game_data->field[y * FIELD_WIDTH + x];
			if (is_filled) {
				Vector2 position = scale_multiply({(float)x, (float)y}, CELL_SIZE_POSITION) + cell_offset_figure;
				draw_rectangle(image, position, ROTATION_VECTOR, CELL_SIZE_FIGURE, color_field);
			}
		}
	}
	
	// draw figure
	if (game_data->has_figure) {
		for (int32 i = 0; i < FIGURE_SIZE; ++i) {
			Vector2i position_i = game_data->figure[i] + game_data->position;
			Vector2 position = scale_multiply({(float)position_i.x, (float)position_i.y}, CELL_SIZE_POSITION) + cell_offset_figure;
			draw_rectangle(image, position, ROTATION_VECTOR, CELL_SIZE_FIGURE, color_figure);
		}
	}
}

GAME_OUTPUT_SOUND(game_output_sound) {
	auto game_data = get_game_data(platform_data);
}
