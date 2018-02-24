void create_rect_to_draw(Ecs_World *ecs_world, Vector2 position, Vector2 size, Vector4 color) {
	auto entity = create_entity(ecs_world);
	entity_add_transform(ecs_world, entity, {position, {1, 0}});
	entity_add_renderer(ecs_world, entity, {-size / 2, size, color});
}

void init_test_entities(Ecs_World *ecs_world) {
	PERSISTENT_LOCAL_VAR bool initialized = false;
	if (!initialized) {
		initialized = true;

		create_rect_to_draw(ecs_world, {0, 0}, {50, 150}, {0, 1, 0, 1});
		create_rect_to_draw(ecs_world, {0, 0}, {150, 50}, {1, 0, 1, 1});
	}
}
