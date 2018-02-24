/*
I use Vector2(x, y)

Can represent 2d rotation

Math representation:
complex = x + y * i
complex = e ^ (angle * i)
complex = cos(angle) + i * sin(angle)

i * i = -1

Rotation formula (by angle A)
> > V' = e ^ (A * i) * V
*/

typedef Vector2 Complex;

inline Complex complex_from_radians(float radians) {
	return {cosine(radians), sine(radians)};
}

inline Complex complex_reciprocal(Complex value) {
	float denominator = magnitude_squared(value);
	return {value.x / denominator, -value.y / denominator};
}

inline Complex complex_multiply(Complex first, Complex second) {
	return {
		first.x * second.x - first.y * second.y,
		first.x * second.y + first.y * second.x
	};
}
