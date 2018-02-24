// #include <iostream> // does not work with the flag '-EHa-', use '-EHsc'
#include "input_keyboard.h"
#include "input_pointer.h"

//
// Platform to game API.
//

#define DLL_EXPORT extern "C" __declspec(dllexport)

#define GAME_UPDATE(ROUTINE_NAME)       void ROUTINE_NAME(Platform_Data *platform_data)
#define GAME_RENDER(ROUTINE_NAME)       void ROUTINE_NAME(Platform_Data *platform_data)
#define GAME_OUTPUT_SOUND(ROUTINE_NAME) void ROUTINE_NAME(Platform_Data *platform_data, int32 samples_count)

struct Time {
	float target_delta;
	float delta;
};

struct RGBA_Offsets {
	int32 red, green, blue, alpha;
};

struct RGBA_Data {
	uint32       *data;
	Vector2i     size;
	RGBA_Offsets offsets;
};

struct Sound_Data {
	int16 *data;
	uint32 size;
	int32 channels;
	int32 samples_per_second;
};

#define PLATFORM_READ_FILE(ROUTINE_NAME) Memory_Chunk ROUTINE_NAME(const char *file_name, Memory_Chunk *memory)
typedef PLATFORM_READ_FILE(read_file_type);

struct Platform_Data {
	// pre-allocated memory chunks
	Memory_Chunk   permanent_memory;
	Memory_Chunk   transient_memory;
	// data from platform to application layer
	Time           time;
	Input_Keyboard input_keyboard;
	Input_Pointer  input_pointer;
	// data from application to platform layer
	bool           keep_alive;
	Vector2i       size_target;
	RGBA_Data      render_buffer_image;
	Sound_Data     sound_buffer_sound;
	// platform-dependent functions
	read_file_type *read_file;
};

//
// Convenience routines
//

inline bool bits_are_set(int32 container, int32 bits) {
	return (container & bits) == bits;
}

inline bool get_bit_at_index(int32 container, int32 index) {
	return bits_are_set(container, 1 << index);
}

inline float uint8_to_fraction(uint8 value) {
	return value / 255.f;
}

inline float int16_to_fraction(int16 value) {
	return (value > 0) ? (value / 32767.f) : (value / 32768.f);
}
