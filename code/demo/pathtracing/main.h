#include "shared/software_renderer_simd.h"
// #include "shared/random_lehmer.h"
#include "shared/random_xorshift32.h"

#include "game_platform.h"
#include "game_data.h"

#include "sphere.h"

#include "scene_1.h"
#include "scene_common.h"


DLL_EXPORT GAME_UPDATE(game_update) {
	permanent_memory = &platform_data->permanent_memory;
	transient_memory = &platform_data->transient_memory;
	input_keyboard   = &platform_data->input_keyboard;

	auto game_data = get_game_data();
	reset_memory_chunk(transient_memory);
	
	platform_data->size_target = {320, 180};
}

DLL_EXPORT GAME_RENDER(game_render) {
	image = platform_data->render_buffer_image;
	// clear_buffer(image, {0, 0, 0, 0});

	auto game_data = get_game_data();
	
	draw_hit_spheres(game_data->camera_position, game_data->camera_rotation);
}

// DLL_EXPORT GAME_OUTPUT_SOUND(game_output_sound) {
// 	auto game_data = get_game_data();
// }