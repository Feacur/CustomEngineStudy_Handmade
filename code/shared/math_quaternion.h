/*
I use Vector4(x, y, z, w)

Can represent 3d rotation

Math representation:
quaternion = (w + x * i) + (y + z * i) * j
quaternion = w + (x * i) + (y * j) + (z * k)
quaternion = e ^ (angle * axis)
quaternion = cos(angle) + axis * sin(angle)

(i * i) = (j * j) = (k * k) = (i * j * k) = -1

(i * j) = -(j * i) = k
(j * k) = -(k * j) = i
(k * i) = -(i * k) = j

Rodrigues' rotation formula (by angle A around axis N)
> V is parallel to axis N
> > V' = V

> V is perpendicular to axis N
> > V' = V * cos(A) + (N x V) * sin(A)

> V is arbitrary to axis N
> > V = V_par + V_perp
> > V' = V_par + V_perp'
> > V' = V * cos(A) + (N x V) * sin(A) + N * (N * V) * (1 - cos(A))
> > as you see, it's a bit cumbersome and requires trigonometric operations

Quaternion rotation formula (by angle A around axis N)
> V is parallel to axis N
> > V' = V

> V is perpendicular to axis N
> > V' = e ^ (A * N) * V

> V is arbitrary to axis N
> > V = V_par + V_perp
> > V' = V_par + V_perp'
> > V' = e ^ (A * N / 2) * V * e ^ (- A * N / 2)
> > as you see, it's concise and avoids trigonometric operations
> > also that's why we specifically use (half_radians = euler_radians / 2) in the code
*/

typedef Vector4 Quaternion;

inline Quaternion quaternion_from_axis(Vector3 axis, float radians) {
	float half_radians = radians / 2;
	float s = sine(half_radians);
	float c = cosine(half_radians);
	return {axis.x * s, axis.y * s, axis.z * s, c};
}

constexpr inline Quaternion quaternion_multiply(Quaternion first, Quaternion second) {
	return {
		cross_product(first.xyz, second.xyz) + first.xyz * second.w + second.xyz * first.w,
		first.w * second.w - dot_product(first.xyz, second.xyz)
	};
}

/*
This code is a result of expanding following expression,
excluding stuff negated by multiplication with zero

return quaternion_multiply(
	quaternion_multiply(
		quaternion_from_axis({0, 1, 0}, euler_radians.y)
		quaternion_from_axis({1, 0, 0}, euler_radians.x)
	),
	quaternion_from_axis({0, 0, 1}, euler_radians.z),
);
*/
inline Quaternion quaternion_from_radians(Vector3 euler_radians) {
	Vector3 half_radians = euler_radians / 2;

	Quaternion x_rotation = {sine(half_radians.x), 0, 0, cosine(half_radians.x)};
	Quaternion y_rotation = {0, sine(half_radians.y), 0, cosine(half_radians.y)};
	Quaternion z_rotation = {0, 0, sine(half_radians.z), cosine(half_radians.z)};

	return quaternion_multiply(quaternion_multiply(y_rotation, x_rotation), z_rotation);
};

/*
Basically creates negative rotation quaternion
*/
constexpr inline Quaternion quaternion_reciprocal(Quaternion value) {
	Quaternion result = {-value.x, -value.y, -value.z, value.w};
	return result / dot_product(value, value);
}

/*
This code is a result of expanding following expression,
excluding stuff negated by multiplication with zero

return quaternion_multiply(
	quaternion_multiply(quaternion, {vector, 0}),
	quaternion_reciprocal(quaternion)
);
*/
constexpr inline Vector3 quaternion_rotate_vector(Quaternion quaternion, Vector3 vector) {
	Quaternion reciprocal = quaternion_reciprocal(quaternion);
	
	Vector3 product_axis = cross_product(quaternion.xyz, vector) + vector * quaternion.w;
	return cross_product(product_axis, reciprocal.xyz)
		+ product_axis * reciprocal.w
		- reciprocal.xyz * dot_product(quaternion.xyz, vector);
}

/*
This code is a result of expanding following expressions,
excluding stuff negated by multiplication with zero

right   = quaternion_rotate_vector(quaternion, {1, 0, 0});
up      = quaternion_rotate_vector(quaternion, {0, 1, 0});
forward = quaternion_rotate_vector(quaternion, {0, 0, 1});
*/
inline void quaternion_get_axes(Quaternion quaternion, Vector3 &right, Vector3 &up, Vector3 &forward) {
	Quaternion reciprocal = quaternion_reciprocal(quaternion);

	Vector3 product_axis_a = {quaternion.w, quaternion.z, -quaternion.y};
	right = cross_product(product_axis_a, reciprocal.xyz)
		+ product_axis_a * reciprocal.w
		- reciprocal.xyz * quaternion.x;

	Vector3 product_axis_b = {-quaternion.z, quaternion.w, quaternion.x};
	up = cross_product(product_axis_b, reciprocal.xyz)
		+ product_axis_b * reciprocal.w
		- reciprocal.xyz * quaternion.y;

	Vector3 product_axis_c = {quaternion.y, -quaternion.x, quaternion.w};
	forward = cross_product(product_axis_c, reciprocal.xyz)
		+ product_axis_c * reciprocal.w
		- reciprocal.xyz * quaternion.z;
};
