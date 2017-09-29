/*
This code is a result of expanding following expression,
which removes one multiplication from the computations

auto p12 = interpolate(p1, p2, fraction);
auto p23 = interpolate(p2, p3, fraction);
return interpolate(p12, p23, fraction);
*/
#define BEZIER_3(TYPE)\
constexpr inline TYPE interpolate(TYPE p1, TYPE p2, TYPE p3, float fraction) {\
	auto d12 = p2 - p1;\
	auto d23 = p3 - p2;\
	auto d123 = interpolate(d12, d23, fraction);\
	return p1 + (d12 + d123) * fraction;\
}

BEZIER_3(float)
BEZIER_3(Vector2)
BEZIER_3(Vector3)
BEZIER_3(Vector4)
