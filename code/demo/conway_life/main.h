#include "../../shared/software_renderer_simd.h"

#include "data.h"

GAME_UPDATE(game_update) {
	platform_data->size_target = {
		(int32)(FIELD_WIDTH * CELL_SIZE_POSITION.x),
		(int32)(FIELD_HEIGHT * CELL_SIZE_POSITION.y)
	};

	auto game_data = get_game_data(platform_data);
	reset_memory_chunk(&platform_data->transient_memory);

	// clear field
	bool should_clear_field = keyboard_get_transition_pressed(&platform_data->input_keyboard, Keyboard_Keys::Escape);
	if (should_clear_field) {
		reset_field(game_data);
	}

	// toggle cells
	bool should_toggle_cell = pointer_get_transition_pressed(&platform_data->input_pointer, Pointer_Keys::Left);
	if (should_toggle_cell) {
		Vector2i cell = {
			(int32)(platform_data->input_pointer.current_position.x / CELL_SIZE_POSITION.x),
			(int32)(platform_data->input_pointer.current_position.y / CELL_SIZE_POSITION.y),
		};
		set_field_cell(game_data, cell, !game_data->field[cell.y * FIELD_WIDTH + cell.x]);
	}

	// toggle active
	bool should_toggle_activity = keyboard_get_transition_pressed(&platform_data->input_keyboard, Keyboard_Keys::Space);
	if (should_toggle_activity) {
		game_data->is_active = !game_data->is_active;
		game_data->elapsed_time = 0;
	}

	// update field
	if (game_data->is_active) {
		game_data->elapsed_time += platform_data->time.delta;
		if (game_data->elapsed_time > TIME_STEP) {
			game_data->elapsed_time -= TIME_STEP;
			update_field(game_data);
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
	
	// draw pointer
	Vector2 pointer_position = {
		(float)platform_data->input_pointer.current_position.x - CELL_SIZE_FIGURE.x / 2,
		(float)platform_data->input_pointer.current_position.y - CELL_SIZE_FIGURE.y / 2
	};
	draw_rectangle(image, pointer_position, ROTATION_VECTOR, CELL_SIZE_FIGURE, color_pointer);
}

GAME_OUTPUT_SOUND(game_output_sound) {
	auto game_data = get_game_data(platform_data);
}
