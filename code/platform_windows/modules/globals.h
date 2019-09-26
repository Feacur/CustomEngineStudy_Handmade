static Vector2i monitor_size;
static Vector2i window_size;

static Render_Settings render_settings;

static POINT os_input_pointer_screen;
static POINT os_input_pointer_client;

inline Vector2i get_pointer_window(POINT point) {
	return {point.x, window_size.y - (point.y + 1)};
}

inline bool pointer_is_inside() {
	return os_input_pointer_client.x >= 0
		&& os_input_pointer_client.y >= 0
		&& os_input_pointer_client.x <  window_size.x
		&& os_input_pointer_client.y <  window_size.y;
}
