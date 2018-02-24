#define C_ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

template<typename T>
struct Array {
	T* data;
	mem_i size, used;
};
