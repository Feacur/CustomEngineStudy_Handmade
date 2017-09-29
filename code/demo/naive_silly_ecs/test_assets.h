#include "shared/image_bitmap.h"

GLOBAL_VARIABLE RGBA_Data test_texture;

void init_test_assets(Platform_Data *data) {
	PERSISTENT_LOCAL_VARIABLE bool initialized = false;
	if (!initialized) {
		initialized = true;
		
		test_texture = read_bitmap_file("test.bmp", data);
		if (!test_texture.data) { test_texture = error_texture; }
	}
}
