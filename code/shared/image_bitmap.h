// From bottom left to top right for positive height
// From top left to bottom right for negative height

// Tightly packed structures
#pragma pack(push, 1)
struct Bitmap_File_Header {
	uint8	file_type_1; // File type part, always 'B'
	uint8	file_type_2; // File type part, always 'M'
	uint32	file_size;   // Size of the file in bytes
	uint16	reserved_1;  // Always 0
	uint16	reserved_2;  // Always 0
	uint32	data_offset; // Starting position of image data in bytes
};

struct Bitmap_Info_Header {
	// MARK: Windows 2.x        // Size = 12
	uint32	this_size;          // Size of this header in bytes
	int32	width;              // Image width in pixels (Win 2.x had int16)
	int32	height;             // Image height in pixels (Win 2.x had int16)
	uint16	planes;             // Number of color planes, which is 1 for BMP
	uint16	bits_per_pixel;     // Number of bits per pixel
	// MARK: Windows 3.x        // Size = 40
	uint32	compression;        // Compression methods used
	uint32	size_of_bitmap;     // Size of bitmap in bytes
	int32	h_pixels_per_meter; // Horizontal resolution in pixels per meter
	int32	v_pixels_per_meter; // Vertical resolution in pixels per meter
	uint32	colors_used;        // Number of colors in the image
	uint32	colors_important;   // Minimum number of important colors
	// MARK: Windows 4.x        // Size = 108
	uint32	red_mask;           // Mask identifying bits of red component
	uint32	green_mask;         // Mask identifying bits of green component
	uint32	blue_mask;          // Mask identifying bits of blue component
	uint32	alpha_mask;         // Mask identifying bits of alpha component
};
#pragma pack(pop)

inline int32 get_mask_offset(uint32 value) {
	int32 bits_in_value = sizeof(value) * 8;
	for (int32 i = 0; i < bits_in_value; ++i) {
		if (get_bit_at_index(value, i)) {
			return i;
		}
	}
	return 0;
}

RGBA_Data read_bitmap_image(Memory_Chunk file_memory, Memory_Chunk *destination_memory) {
	RGBA_Data result = {};
	
	auto file_header = (Bitmap_File_Header *)file_memory.data;
	if (file_header->file_type_1 != 'B') { return result; }
	if (file_header->file_type_2 != 'M') { return result; }
	
	auto info_header = (Bitmap_Info_Header *)(file_header + 1);
	if (info_header->planes != 1) { return result; }
	if (info_header->compression != 3) { return result; }
	
	result.size = {info_header->width, info_header->height};
	int32 pixels_count = result.size.x * result.size.y;
	
	result.data = ALLOCATE_ARRAY(destination_memory, uint32, pixels_count);
	
	result.offsets.red   = get_mask_offset(info_header->red_mask);
	result.offsets.green = get_mask_offset(info_header->green_mask);
	result.offsets.blue  = get_mask_offset(info_header->blue_mask);
	result.offsets.alpha = get_mask_offset(info_header->alpha_mask);
	
	uint32 *source = (uint32 *)(file_memory.data + file_header->data_offset);
	uint32 *destination = result.data;
	
	for (int32 i = 0; i < pixels_count; ++i) {
		*(result.data + i) = *(source + i);
	}
	
	return result;
}

RGBA_Data read_bitmap_file(const char* file_name, Platform_Data *data) {
	Memory_Chunk file_memory = data->read_file(file_name, &data->transient_memory);
	if (file_memory.data) {
		return read_bitmap_image(file_memory, &data->permanent_memory);
	}
	return {};
}
