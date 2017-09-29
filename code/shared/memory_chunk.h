#include <string.h> // memset, memcpy

struct Memory_Chunk {
	uint8 *data;
	mem_i size, used;
};

inline void reset_memory_chunk(Memory_Chunk *memory) {
	memory->used = 0;
	memset(memory->data, 0, memory->size);
}

inline Memory_Chunk allocate_memory_chunk(Memory_Chunk *memory, mem_i size_in_bytes) {
	Memory_Chunk result = {};
	if ((memory->used + size_in_bytes) <= memory->size) {
		result.data = memory->data + memory->used;
		result.size = size_in_bytes;

		memory->used = memory->used + size_in_bytes;
	}
	return result;
}

#define ALLOCATE_STRUCT(MEMORY, TYPE) (TYPE *)allocate_memory_chunk(MEMORY, sizeof(TYPE)).data
#define ALLOCATE_ARRAY(MEMORY, TYPE, COUNT) (TYPE *)allocate_memory_chunk(MEMORY, sizeof(TYPE) * COUNT).data
