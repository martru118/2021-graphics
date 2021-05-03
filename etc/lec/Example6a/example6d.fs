#version 420 core
/*
 *  Simple fragment sharder for laboratory two
 */

in vec3 normal;
in vec4 position;
uniform vec3 eye;

layout(std140, binding=1) uniform Light {
	vec4 Lposition;
	vec4 Lcolour;
	vec4 spotDirection;
	float spotCutoff;
	float spotExp;
};

void main() {
	vec3 N;
	vec4 colour = vec4(1.0, 0.0, 0.0, 1.0);
	float spotCos;
	float atten;
	vec3 H;
	float diffuse;
	float specular;
	float n = 100.0;
	vec3 L;

	N = normalize(normal);
	L = vec3(Lposition) - position.xyz;
	H = normalize(L + eye);
	L = vec3(normalize(L));
	spotCos = dot(L, vec3(normalize(spotDirection)));
	if(spotCos < spotCutoff) {
		atten = 0;
	} else {
		atten = pow(spotCos,spotExp);
	}
	diffuse = dot(N,L) * atten;
	if(diffuse < 0.0) {
		diffuse = 0.0;
		specular = 0.0;
	} else {
		specular = pow(max(0.0, dot(N,H)),n) * atten;
	}

	gl_FragColor = min(0.3*colour + diffuse*colour*Lcolour + Lcolour*specular, vec4(1.0));
	gl_FragColor.a = colour.a;
}