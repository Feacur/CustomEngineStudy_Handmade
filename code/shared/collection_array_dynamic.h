template<typename T>
struct Array_Dynamic {
	T * data;
	size_t capacity, length;

	inline T & operator[](size_t index) { return data[index]; }
	inline T const & operator[](size_t index) const { return data[index]; }

	inline void add(T const & item) { data[length++] = item; }
	inline void add() { length++; }

	typedef T * allocate_func(size_t count);
	inline void set_capacity(size_t value, allocate_func * allocate) {
		data = (*allocate)(value);
		capacity = value;
	}
};

template<typename T>
bool contains(Array_Dynamic<T> array, size_t index) {
	for (size_t i = 0; i < array.length; ++i) {
		if (array[i] == index) { return true; }
	}
	return false;
}

template<typename T>
size_t get_index_minimum(Array_Dynamic<T> array) {
	size_t index = 0;
	for (size_t i = 1; i < array.length; ++i) {
		if (array[index] > array[i]) {
			index = i;
		}
	}
	return index;
}
