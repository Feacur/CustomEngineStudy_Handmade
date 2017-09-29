void render_world(RGBA_Data image, Ecs_World ecs_world) {
	Vector2 screen_offset = {(float)image.size.x / 2, (float)image.size.y / 2};

	clear_buffer(image, {0, 0, 0, 0});

	for(int32 i = 0; i < ecs_world.used; i++) {
		auto entity    = ecs_world.entities + i;
		auto transform = entity_get_transform(&ecs_world, entity);
		auto renderer  = entity_get_renderer(&ecs_world, entity);

		Vector2 position    = transform->position + renderer->offset + screen_offset;
		Vector2 rotation    = transform->rotation;
		draw_rectangle(image, position, rotation, renderer->size, renderer->color);
	}
}
