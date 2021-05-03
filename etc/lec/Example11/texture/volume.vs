in vec4 vPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 texCoord;

void main(void) {

	gl_Position = projection * view * model * vPosition;
	texCoord = (model * vPosition).xyz;

}
