#define C_ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

template<typename T>
struct Array {
    Memory_Chunk memory_chunk;
};
