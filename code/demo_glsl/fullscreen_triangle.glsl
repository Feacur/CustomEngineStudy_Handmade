// full
// https://twitter.com/Atrix256/status/1093388452211580928
// https://rauwendaal.net/2014/06/14/rendering-a-screen-covering-triangle-in-opengl/

#shader vertex
#version 330 core

void main() {
	float x = -1.0 + float((gl_VertexID & 1) << 2);
	float y = -1.0 + float((gl_VertexID & 2) << 1);
	gl_Position = vec4(x, y, 0, 1);
}

void main_as_lookup() {
	const vec4 vert_pos[3] = vec4[](
		vec4(-1, -1, 0, 1),
		vec4(3, -1, 0, 1),
		vec4(-1, 3, 0, 1)
	);
	gl_Position = vert_pos[gl_VertexID % 3];
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main() {
	color = vec4(1.0, 1.0, 1.0, 1.0);
}