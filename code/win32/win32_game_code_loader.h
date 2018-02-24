typedef GAME_UPDATE(game_update_type);
typedef GAME_RENDER(game_render_type);
typedef GAME_OUTPUT_SOUND(game_output_sound_type);

GAME_UPDATE(game_update_stub) { }
GAME_RENDER(game_render_stub) { }
GAME_OUTPUT_SOUND(game_output_sound_stub) { }

struct Game_Code {
	HMODULE                library;
	FILETIME               creation_time;
	game_render_type       *game_render;
	game_output_sound_type *game_output_sound;
	game_update_type       *game_update;
};

//
// Routines
//

#define LOAD_PROCEDURE(ROUTINE_NAME)\
if (result.library) {\
	result.ROUTINE_NAME = (ROUTINE_NAME##_type *)GetProcAddress(result.library, #ROUTINE_NAME);\
}\
if (!result.ROUTINE_NAME) {\
	LOG_WARNING("Can't find \"" #ROUTINE_NAME "\" handler in the game code library");\
	result.ROUTINE_NAME = &ROUTINE_NAME##_stub;\
}

Game_Code load_game_code(const char *file_name) {
	Game_Code result = {};

	result.library = LoadLibraryA(file_name);

	LOAD_PROCEDURE(game_update)
	LOAD_PROCEDURE(game_render)
	LOAD_PROCEDURE(game_output_sound)

	return result;
}

#undef LOAD_PROCEDURE

void reload_game_code(Game_Code *code) {
	PERSISTENT_LOCAL_CONST char *build_library_name   = "elementary.dll";
	PERSISTENT_LOCAL_CONST char *runtime_library_name = "elementary_runtime.dll";
	
	WIN32_FIND_DATA find_data;
	HANDLE file = FindFirstFileA(build_library_name, &find_data);
	ASSERT_TRUE(file != INVALID_HANDLE_VALUE, "Can't find \"elementary.dll\"");

	auto creation_time = find_data.ftLastWriteTime;
	FindClose(file);

	if (CompareFileTime(&code->creation_time, &creation_time) != 0) {
		FreeLibrary(code->library);
		CopyFile(build_library_name, runtime_library_name, 0);
		*code = load_game_code(runtime_library_name);
		code->creation_time = creation_time;

		LOG_TRACE("Reloaded game code library");
	}
}
