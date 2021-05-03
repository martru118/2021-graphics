in vec3 texCoord;
uniform sampler3D tex;

void main(void) {

	gl_FragColor = texture(tex, texCoord);

}
