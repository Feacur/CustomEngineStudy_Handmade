struct Game_Data {
	Ecs_World ecs_world;
};

Game_Data *get_game_data(Platform_Data *platform_data) {
	PERSISTENT_LOCAL_VAR Game_Data *game_data = 0;
	if (!game_data) {
		auto memory = &platform_data->permanent_memory;
		game_data = ALLOCATE_STRUCT(memory, Game_Data);
    	
        auto ecs_world = &game_data->ecs_world;
        ecs_world->size = 1024;
        ecs_world->used = 0;

        ecs_world->entities             = ALLOCATE_ARRAY(memory, Entity,    ecs_world->size);

        ecs_world->components_transform = ALLOCATE_ARRAY(memory, Transform, ecs_world->size);
        ecs_world->components_renderer  = ALLOCATE_ARRAY(memory, Renderer,  ecs_world->size);
        ecs_world->components_collider  = ALLOCATE_ARRAY(memory, Collider,  ecs_world->size);
        ecs_world->components_rigidbody = ALLOCATE_ARRAY(memory, Rigidbody, ecs_world->size);
	}

	return game_data;
}
