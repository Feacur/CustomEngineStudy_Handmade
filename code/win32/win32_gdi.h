struct Render_Buffer {
	RGBA_Data  image;
	BITMAPINFO info;
	Vector2i   size_target;
	Vector2i   size_window;
	Vector2i   size_render;
};

GLOBAL_VAR Render_Buffer render_buffer;

Vector2i window_to_buffer(Vector2i point) {
	auto destination = (render_buffer.size_window - render_buffer.size_render) / 2;
	
	float aspect_buffer = (float)render_buffer.size_target.x / render_buffer.size_target.y;
	float aspect_window = (float)render_buffer.size_window.x / render_buffer.size_window.y;
	
	if (aspect_buffer < aspect_window) {
		return (point - destination) * render_buffer.size_target.y / render_buffer.size_window.y;
	}
	else {
		return (point - destination) * render_buffer.size_target.x / render_buffer.size_window.x;
	}
}

void make_render_buffer_fit(HWND window) {
	RECT client_rect;
	GetClientRect(window, &client_rect);
	render_buffer.size_window = {
		client_rect.right, client_rect.bottom
	};
	
	float aspect_buffer = (float)render_buffer.size_target.x / render_buffer.size_target.y;
	float aspect_window = (float)render_buffer.size_window.x / render_buffer.size_window.y;
	
	if (aspect_buffer > aspect_window) {
		render_buffer.size_render = {
			render_buffer.size_window.x,
			(int32)(render_buffer.size_window.x / aspect_buffer)
		};
	}
	else {
		render_buffer.size_render = {
			(int32)(render_buffer.size_window.y * aspect_buffer),
			render_buffer.size_window.y
		};
	}
}

void set_render_buffer_size(Vector2i size) {
	if (render_buffer.image.data) {
		free_memory(render_buffer.image.data);
	}
	
	size.x = round_up_with_step(size.x, 4);
	size.y = round_up_with_step(size.y, 4);

	// Windows GDI expects ARGB image format
	render_buffer.image.offsets.red   = 16;
	render_buffer.image.offsets.green = 8;
	render_buffer.image.offsets.blue  = 0;
	render_buffer.image.offsets.alpha = 24;
	
	const int32 bytes_per_pixel = sizeof(uint32);
	render_buffer.image.data = (uint32 *)allocate_memory(size.x * size.y * bytes_per_pixel);
	ASSERT_TRUE(render_buffer.image.data, "Can't allocate render buffer image memory");
	
	render_buffer.image.size = size;
	
	render_buffer.info.bmiHeader.biSize        = sizeof(render_buffer.info.bmiHeader);
	render_buffer.info.bmiHeader.biWidth       = size.x;
	render_buffer.info.bmiHeader.biHeight      = size.y;
	render_buffer.info.bmiHeader.biPlanes      = 1;
	render_buffer.info.bmiHeader.biBitCount    = (WORD)(8 * bytes_per_pixel);
	render_buffer.info.bmiHeader.biCompression = BI_RGB;
}

void display_blackness(HDC device_context) {
	BitBlt(
		// destination
		device_context,
		0, 0, render_buffer.size_window.x, render_buffer.size_window.y,
		// settings
		0, 0, 0,
		BLACKNESS
	);
}

void reinit_render_buffer(HWND window, Vector2i size) {
	if (render_buffer.size_target.x != size.x || render_buffer.size_target.y != size.y) {
		render_buffer.size_target = size;

		set_render_buffer_size(size);
		make_render_buffer_fit(window);

		HDC device_context = GetDC(window);
		display_blackness(device_context);

		// SetStretchBltMode(device_context, STRETCH_DELETESCANS);
		SetStretchBltMode(device_context, STRETCH_HALFTONE);
		
		LOG_TRACE("Reinitialized GDI parameters");
	}
}

void display_render_buffer(HDC device_context) {
	auto destination = (render_buffer.size_window - render_buffer.size_render) / 2;

	StretchDIBits(
		// destination
		device_context,
		destination.x, destination.y,
		render_buffer.size_render.x, render_buffer.size_render.y,
		// source
		0, 0,
		render_buffer.size_target.x, render_buffer.size_target.y,
		render_buffer.image.data,
		// settings
		&render_buffer.info,
		DIB_RGB_COLORS, SRCCOPY
	);
}
