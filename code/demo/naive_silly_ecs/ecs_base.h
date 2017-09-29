#define NULL_ENTITY_ID 0
#define NULL_COMPONENT_ID 0
#define COMPONENTS_COUNT 4

struct Entity {
	int32  id;
	int32  parent_id;
	bool components[COMPONENTS_COUNT];
};
