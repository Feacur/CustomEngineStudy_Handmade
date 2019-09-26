static uint8 const checksum[8] = {'p', 'h', 'y', 's', 'i', 'c', 's', ' '};

//
// data layout
//

struct Entity {
	Vector2 position;
	Complex rotation;
	Vector2 size;
	Vector2 velocity;
	Vector2 acceleration;
};

struct Game_Data {
	uint8                 checksum[8];
	uint32                random_state;
	// actual data
	Array_Dynamic<Entity> entities;
};

//
// game settings
//

static uint8 const ENTITIES_COUNT = 1;

//
// rendering settings
//

static Vector4 const ENTITY_COLOR = {0.8f, 0.6f, 0.4f, 1.0f};
