#include "software_renderer_base.h"

inline void draw_rectangle(RGBA_Data image, Vector2 position, Vector2 orientation, Vector2 size, Vector4 color) {
	Vector2 axis_x = orientation;
	Vector2 axis_y = {-axis_x.y, axis_x.x};
	
	axis_x = axis_x / size.x;
	axis_y = axis_y / size.y;
	
	Rendering_Rect rect = create_rendering_rect(position, orientation, size);
	rect = restrict_rendering_rect_to_image(rect, image);
	
	Vector4 source = color;
	
	for (int32 y = rect.bottom; y < rect.top; ++y) {
		float local_y = (float)y - position.y;
		uint32 *destination_line = image.data + y * image.size.x;
		for (int32 x = rect.left; x < rect.right; ++x) {
			float local_x = (float)x - position.x;
			
			Vector2 local_position = {local_x, local_y};
			Vector2 UV = {
				dot_product(local_position, axis_x),
				dot_product(local_position, axis_y)
			};
			
			bool inside = (UV.x >= 0) && (UV.y >= 0) && (UV.x < 1) && (UV.y < 1);
			
			if (inside) {
				uint32 *destination_uint32 = destination_line + x;
				Vector4 destination = color32_to_vector4(*destination_uint32, image.offsets);
				
				Vector4 blend = interpolate(destination, source, source.w);
				*destination_uint32 = vector4_to_color32(blend, image.offsets);
			}
		}
	}
}

inline void draw_rectangle(RGBA_Data image, Vector2 position, Vector2 orientation, Vector2 size, RGBA_Data texture) {
	Vector2 axis_x = orientation;
	Vector2 axis_y = {-axis_x.y, axis_x.x};
	
	axis_x = axis_x / size.x;
	axis_y = axis_y / size.y;
	
	Rendering_Rect rect = create_rendering_rect(position, orientation, size);
	rect = restrict_rendering_rect_to_image(rect, image);
	
	for (int32 y = rect.bottom; y < rect.top; ++y) {
		float local_y = (float)y - position.y;
		uint32 *destination_line = image.data + y * image.size.x;
		for (int32 x = rect.left; x < rect.right; ++x) {
			float local_x = (float)x - position.x;
			
			Vector2 local_position = {local_x, local_y};
			Vector2 UV = {
				dot_product(local_position, axis_x),
				dot_product(local_position, axis_y)
			};
			
			bool inside = (UV.x >= 0) && (UV.y >= 0) && (UV.x < 1) && (UV.y < 1);
			
			if (inside) {
				int32 texture_x  = (int32)(texture.size.x * UV.x);
				int32 texture_y  = (int32)(texture.size.y * UV.y);
				
				uint32 *source_uint32 = texture.data + texture_y * texture.size.x + texture_x;
				Vector4 source = color32_to_vector4(*source_uint32, texture.offsets);
				
				uint32 *destination_uint32 = destination_line + x;
				Vector4 destination = color32_to_vector4(*destination_uint32, image.offsets);
				
				Vector4 blend = interpolate(destination, source, source.w);
				*destination_uint32 = vector4_to_color32(blend, image.offsets);
			}
		}
	}
}
