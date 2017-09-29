struct Rendering_Rect {
	int32 left, right, bottom, top;
};

inline Rendering_Rect create_rendering_rect(Vector2 position, Vector2 orientation, Vector2 size) {
	Vector2 axis_x = orientation;
	Vector2 axis_y = {-axis_x.y, axis_x.x};
	
	Vector2 edge_x = axis_x * size.x;
	Vector2 edge_y = axis_y * size.y;
	
	float left   = INFINITY;
	float bottom = INFINITY;
	float right  = -INFINITY;
	float top    = -INFINITY;
	
	Vector2 local_vertices[] = {{0, 0}, edge_x, edge_y, edge_x + edge_y};
	int32 elements_in_local_vertices = C_ARRAY_LENGTH(local_vertices);
	for (int32 i = 0; i < elements_in_local_vertices; ++i) {
		Vector2 vertex = position + local_vertices[i];
		if (left   > vertex.x) { left   = vertex.x; }
		if (bottom > vertex.y) { bottom = vertex.y; }
		if (right  < vertex.x) { right  = vertex.x; }
		if (top    < vertex.y) { top    = vertex.y; }
	}
	
	return {
		(int32)left, (int32)right,
		(int32)bottom, (int32)top
	};
}

inline Rendering_Rect restrict_rendering_rect_to_image(Rendering_Rect rect, RGBA_Data image) {
	rect.left   = ((rect.left - 3) / 4) * 4;
	rect.right  = ((rect.right + 3) / 4) * 4;
	rect.bottom = rect.bottom - 1;
	rect.top    = rect.top + 1;
	
	rect.left   = clamp(rect.left,   0, image.size.x);
	rect.right  = clamp(rect.right,  0, image.size.x);
	rect.bottom = clamp(rect.bottom, 0, image.size.y);
	rect.top    = clamp(rect.top,    0, image.size.y);
	
	return rect;
}

inline float get_color32_component_as_float(uint32 color32, int offset) {
	uint32 component = (color32 >> offset) & 0xff;
	return (float)component / 0xff;
}

inline Vector4 color32_to_vector4(uint32 color32, RGBA_Offsets offsets) {
	return {
		get_color32_component_as_float(color32, offsets.red),
		get_color32_component_as_float(color32, offsets.green),
		get_color32_component_as_float(color32, offsets.blue),
		get_color32_component_as_float(color32, offsets.alpha)
	};
}

inline uint32 make_color32_component_with_offset(float value, int32 offset) {
	uint32 component = (uint32)(value * 0xff) & 0xff;
	return component << offset;
}

inline uint32 vector4_to_color32(Vector4 color, RGBA_Offsets offsets) {
	return make_color32_component_with_offset(color.x, offsets.red)
		|  make_color32_component_with_offset(color.y, offsets.green)
		|  make_color32_component_with_offset(color.z, offsets.blue)
		|  make_color32_component_with_offset(color.w, offsets.alpha);
}

inline void clear_buffer(RGBA_Data image, Vector4 color) {
	uint32 rgba_color = vector4_to_color32(color, image.offsets);
	int32 pixels_count = image.size.x * image.size.y;
	for(int32 i = 0; i < pixels_count; ++i) {
		*(image.data + i) = rgba_color;
	}
}
