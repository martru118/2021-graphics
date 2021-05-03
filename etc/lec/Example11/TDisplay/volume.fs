#version 330 core

in vec3 texCoord;
in vec3 dir;
uniform sampler3D tex;

void main(void) {
	int slices = 10;
	int i;

	float step = 0.1;
	vec3 coord = texCoord;

	vec4 average = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 minVec = vec4(0.0, 0.0, 1.0, 1.0);
	vec4 maxVec = vec4(0.0, 0.0, 1.0, 1.0);
	vec4 diffVec;

	for(i = 0; i < slices; i++) {
		average += texture(tex, coord);
		minVec = min(minVec, texture(tex, coord));
		maxVec = max(maxVec, texture(tex, coord));
		diffVec = maxVec - texture(tex, coord);

		coord += step*dir;
	}

	average /= slices;
	gl_FragColor = texture(tex, texCoord);

	//gl_FragColor = average;
	//gl_FragColor = minVec;
	//gl_FragColor = maxVec;
	//gl_FragColor = diffVec;
}
