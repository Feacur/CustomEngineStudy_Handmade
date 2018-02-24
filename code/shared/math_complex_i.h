/*
I use Vector2i(x, y)

Can represent 2d rotation

Math representation:
complex = x + y * i
complex = e ^ (angle * i)
complex = cos(angle) + i * sin(angle)

i * i = -1

Rotation formula (by angle A)
> > V' = e ^ (A * i) * V
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
