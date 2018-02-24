GLOBAL_CONST    RGBA_Offsets standard_rgba_offsets = {24, 16, 8, 0};

GLOBAL_VAR Vector4      error_color = {1, 0, 1, 1};
GLOBAL_VAR uint32       error_color32 = vector4_to_color32(error_color, standard_rgba_offsets);
GLOBAL_VAR RGBA_Data    error_texture;

void init_default_assets(Platform_Data *data) {
	PERSISTENT_LOCAL_VAR bool initialized = false;
	if (!initialized) {
		initialized = true;
		
		error_texture.data = &error_color32;
		error_texture.size = {1, 1};
		error_texture.offsets = standard_rgba_offsets;
	}
}
