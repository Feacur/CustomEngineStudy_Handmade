// VSCode extension "circledev.glsl-canvas"

precision lowp float;

uniform vec2  u_resolution;
uniform vec2  u_mouse;
uniform vec3  u_camera;
uniform float u_time;

void main() {
	vec3 color = vec3(
		1.0 - (cos(u_time) + sin(u_time)),
		cos(u_time),
		sin(u_time)
	);
	gl_FragColor = vec4(color, 1.0);
}
