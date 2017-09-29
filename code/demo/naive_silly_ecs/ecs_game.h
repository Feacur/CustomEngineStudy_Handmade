enum struct Component_Type : int32 {
	Transform = 0,
	Renderer  = 1,
	Collider  = 2,
	Rigidbody = 3
};

struct Transform {
	Vector2 position;
	Complex rotation;
};

struct Renderer {
	Vector2 offset;
	Vector2 size;
	Vector4 color;
};

struct Collider {
	Vector2 offset;
	Vector2 size;
};

struct Rigidbody {
	float mass;
	float drag;
	float elasticity;
};

struct Ecs_World {
	int32       size;
	int32       used;
	Entity    *entities;
	Transform *components_transform;
	Renderer  *components_renderer;
	Collider  *components_collider;
	Rigidbody *components_rigidbody;
};


Entity *create_entity(Ecs_World *ecs_world) {
	if (ecs_world->used < ecs_world->size) {
		Entity *entity = ecs_world->entities + ecs_world->used;
		entity->id = ecs_world->used;
		ecs_world->used++;
		return entity;
	}
	return 0;
}

#define ENTITY_ADD_ROUTINE(TYPE, NAME)\
TYPE *entity_add_ ## NAME(Ecs_World *ecs_world, Entity *entity, TYPE value) {\
    entity->components[(int32)Component_Type:: ## TYPE] = true;\
	TYPE *transform = ecs_world->components_ ## NAME + entity->id;\
	*transform = value;\
    return transform;\
}

#define ENTITY_GET_ROUTINE(TYPE, NAME)\
TYPE *entity_get_ ## NAME(Ecs_World *ecs_world, Entity *entity) {\
	bool has_component = entity->components[(int32)Component_Type:: ## TYPE];\
	if (has_component) {\
		TYPE *transform = ecs_world->components_ ## NAME + entity->id;\
		return transform;\
	}\
	return 0;\
}

#define ENTITY_REMOVE_ROUTINE(TYPE, NAME)\
void entity_remove_ ## NAME(Ecs_World *ecs_world, Entity *entity) {\
    entity->components[(int32)Component_Type:: ## TYPE] = false;\
}

ENTITY_ADD_ROUTINE(Transform, transform)
ENTITY_GET_ROUTINE(Transform, transform)
ENTITY_REMOVE_ROUTINE(Transform, transform)

ENTITY_ADD_ROUTINE(Renderer, renderer)
ENTITY_GET_ROUTINE(Renderer, renderer)
ENTITY_REMOVE_ROUTINE(Renderer, renderer)

ENTITY_ADD_ROUTINE(Collider, collider)
ENTITY_GET_ROUTINE(Collider, collider)
ENTITY_REMOVE_ROUTINE(Collider, collider)

ENTITY_ADD_ROUTINE(Rigidbody, rigidbody)
ENTITY_GET_ROUTINE(Rigidbody, rigidbody)
ENTITY_REMOVE_ROUTINE(Rigidbody, rigidbody)
