// #include "shared/software_renderer_plain.h"
#include "shared/software_renderer_simd.h"
#include "shared/assets.h"

#include "ecs_base.h"
#include "ecs_game.h"

#include "game_data.h"
#include "ecs_renderer.h"

#include "test_assets.h"
#include "test_entities.h"

DLL_EXPORT GAME_UPDATE(game_update) {
	// platform_data->size_target = {320, 200};

	auto game_data = get_game_data(platform_data);
	init_default_assets(platform_data);
	init_test_assets(platform_data);
	init_test_entities(&game_data->ecs_world);
}

DLL_EXPORT GAME_RENDER(game_render) {
	auto game_data = get_game_data(platform_data);
	render_world(platform_data->render_buffer_image, game_data->ecs_world);
}

// DLL_EXPORT GAME_OUTPUT_SOUND(game_output_sound) {
// 	auto game_data = get_game_data(platform_data);
// }
