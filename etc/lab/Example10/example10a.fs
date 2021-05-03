#version 330 core

in vec3 normal;
in vec3 position;
in vec2 tc;

uniform samplerCube tex;

uniform vec4 colour;
uniform vec3 Eye;
uniform vec3 light;
uniform vec4 material;

void main() {	
	vec3 tc = reflect(position - Eye, normal);
	gl_FragColor = texture(tex, tc);

	/* 

	float Ro, R;
	float eta = 1.5;

	vec3 incident = normalize(position - Eye);
	vec3 reflection = reflect(incident, normal);
	vec3 refraction = refract(incident, normal, eta);

	Ro = pow((1.0 - eta)/(1.0 + eta), 2.0);
	R = Ro + (1.0 - Ro)*pow(1 - cos(eta), 5.0);

	gl_FragColor = texture(tex, refraction)*R + texture(tex, reflection)*(1 - R);

	*/
}

float rand() {
	float f;

	int num = 1235;
	int a = 141;
	int c = 28411;
	int m = 134456;

	num = (a*num+c) % m;
	f = (num+0.0)/m;

	return((f-0.5)*2.0);
}