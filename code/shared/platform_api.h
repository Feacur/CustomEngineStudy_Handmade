#include <stdio.h> // printf
#include "input_keyboard.h"
#include "input_pointer.h"

//
// Platform to game API.
//

#define GAME_UPDATE(ROUTINE_NAME)       void ROUTINE_NAME(Platform_Data *platform_data)
#define GAME_RENDER(ROUTINE_NAME)       void ROUTINE_NAME(Platform_Data *platform_data)
#define GAME_OUTPUT_SOUND(ROUTINE_NAME) void ROUTINE_NAME(Platform_Data *platform_data)

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
	// platform-dependent functions
	read_file_type *read_file;
};

//
// Debug routines
//

inline void assert_true(bool statement_value, const char* message) {
	if (!statement_value) {
		printf(message);
		*(int *)0 = 0;
	}
}

#define DEBUG_STRINGIFY_VALUE(VALUE) #VALUE
#define DEBUG_STRINGIFY_MACRO(MACRO) DEBUG_STRINGIFY_VALUE(MACRO)
#define DEBUG_AT __FILE__ ":" DEBUG_STRINGIFY_MACRO(__LINE__)
#define ASSERT_TRUE(STATEMENT, MESSAGE) assert_true(STATEMENT, MESSAGE " " DEBUG_AT)

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
