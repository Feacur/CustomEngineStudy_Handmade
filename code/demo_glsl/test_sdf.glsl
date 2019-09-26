// VSCode extension "circledev.glsl-canvas"

precision lowp float;

uniform vec2  u_resolution;
uniform vec2  u_mouse;
uniform vec3  u_camera;
uniform float u_time;

#define AZURE vec3(0.0, 0.5, 1.0)
#define WHITE vec3(1.0, 1.0, 1.0)

float sdf_circle(in vec2 point, in float radius) {
	return length(point) * 2.0 - radius;
}

float sdf_blend_expo(in float a, in float b, in float k) {
	float res = exp(-k * a) + exp(-k * b);
	return -log(res) / k;
}

float clamp01(in float value) {
	return clamp(value, 0.0, 1.0);
}

void main() {
	vec2 pixel = gl_FragCoord.xy - u_resolution * 0.5;

	vec3 circle1 = vec3(sin(u_time * 1.1) * 110.0, sin(u_time * 0.5) * 100.0, 100);
	vec3 circle2 = vec3(cos(u_time * 0.7) * 90.0, cos(u_time * 1.3) * 130.0, 100);
	float distance1 = sdf_circle(circle1.xy - pixel, circle1.z);
	float distance2 = sdf_circle(circle2.xy - pixel, circle2.z);
	float distance = sdf_blend_expo(distance1, distance2, 0.05);

	vec3 color = mix(WHITE, AZURE, clamp01(distance));
	gl_FragColor = vec4(color, 1.0);
}
