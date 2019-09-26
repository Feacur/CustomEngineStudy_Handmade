static uint8 const checksum[8] = {'g', 'a', 'm', 'e', 'l', 'i', 'f', 'e'};

//
// data layout
//

using Field_Cell = uint8;
struct Game_Data {
	uint8                     checksum[8];
	uint32                    random_state;
	// actual data
	Array_Dynamic<Field_Cell> field;
	Vector2i                  field_dimensions;
	// controls
	bool                      is_active;
	int32                     step_time;
};

struct Field_Prefab {
	Array_Dynamic<Field_Cell> tiles;
	int32 width;
};

//
// game settings
//

static int32 const TIME_STEP = 100 * 1000;

//
// rendering settings
//

static Vector2 const CELL_SIZE_POSITION   = {10, 10};
static Vector2 const CELL_SIZE_BACKGROUND = {10, 10};
static Vector2 const CELL_SIZE_FIGURE     = { 8,  8};

static Vector4 const color_background_odd  = {0.25f, 0.25f, 0.25f, 1};
static Vector4 const color_background_even = {0.1f, 0.1f, 0.1f, 1};
static Vector4 const color_field           = {0.8f, 0.8f, 0.8f, 1};
static Vector4 const color_pointer         = {0.2f, 0.6f, 0.8f, 0.5f};
