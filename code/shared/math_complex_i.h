/*
I use Vector2i(x, y)

Can represent 2d rotation

Math representation:
Complexi = x + y * i

i * i = -1
*/

typedef Vector2i Complexi;

inline Complexi complex_reciprocal(Complexi value) {
	auto denominator = magnitude_squared(value);
	return {value.x / denominator, -value.y / denominator};
}

inline Complexi complex_multiply(Complexi first, Complexi second) {
	return {
		first.x * second.x - first.y * second.y,
		first.x * second.y + first.y * second.x
	};
}
