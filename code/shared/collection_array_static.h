template<typename T, size_t capacity>
struct Array_Static {
	T data[capacity];
	size_t length;

	inline T & operator[](size_t index) { return data[index]; }
	inline T const & operator[](size_t index) const { return data[index]; }

	inline void add(T const & item) { data[length++] = item; }
	inline void add() { length++; }
};
