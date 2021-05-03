#version 330 core

in vec4 vPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 texCoord;
out vec3 dir;

void main(void) {
	vec4 position = vPosition;
	gl_Position = projection * view * model * vPosition;
	position.y -= 0.5;

	texCoord = (model * position).xyz;
	dir = (model * vec4(0.0, 1.0, 0.0, 0.0)).xyz;
}
