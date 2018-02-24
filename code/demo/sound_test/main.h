#include "shared/software_renderer_simd.h"

#include "data.h"

DLL_EXPORT GAME_UPDATE(game_update) {
	// platform_data->size_target = {
	// 	(int32)(FIELD_WIDTH * CELL_SIZE_POSITION.x),
	// 	(int32)(FIELD_HEIGHT * CELL_SIZE_POSITION.y)
	// };

	auto game_data = get_game_data(platform_data);
	reset_memory_chunk(&platform_data->transient_memory);
}

DLL_EXPORT GAME_RENDER(game_render) {
	auto image = platform_data->render_buffer_image;
	clear_buffer(image, {0, 0, 0, 0});

	auto game_data = get_game_data(platform_data);
}

DLL_EXPORT GAME_OUTPUT_SOUND(game_output_sound) {
	auto game_data = get_game_data(platform_data);
	
	PERSISTENT_LOCAL_VAR real32 Time = 0;
	PERSISTENT_LOCAL_VAR real32 Volume = 5000;
	
	int16 *SampleOut = platform_data->sound_buffer_sound.data;
	for(int BlockIndex = 0; BlockIndex < samples_count; ++BlockIndex) {
		Time = Time + tau / (platform_data->sound_buffer_sound.samples_per_second / 400.f);
		if (Time > tau) { Time = Time - tau; }
		int16 SampleValue = (int16)(sinf(Time) * Volume);
		for (int ChannelIndex = 0; ChannelIndex < platform_data->sound_buffer_sound.channels; ++ChannelIndex) {
			*SampleOut++ = SampleValue;
		}
	}
}
