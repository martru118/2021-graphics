#version 420 core	// nice

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

layout(std140, binding=2) uniform Material {
	vec4 Mcolour;
	float n;
};

void main() {
	vec3 N;
	vec3 H;
	vec3 L;

	float spotCos;
	float atten;
	float diffuse;
	float specular;

	// point light
	if (spotDirection == vec4(0.0, 0.0, 0.0, 0.0)) {
		N = normalize(normal);
		L = normalize(vec3(Lposition) - position.xyz);
		H = normalize(L + eye);
		L = normalize(L);

		diffuse = dot(N,L);
		if(diffuse < 0.0) {
			diffuse = 0.0;
			specular = 0.0;
		} else {
			specular = pow(max(0.0, dot(N,H)),n);
		}

	// spotlight
	} else {
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
		
		diffuse = dot(N, L) * atten;
		if(diffuse < 0.0) {
			diffuse = 0.0;
			specular = 0.0;
		} else {
			specular = pow(max(0.0, dot(N, H)), n) * atten;
		}
	}

	gl_FragColor = min(0.3*Mcolour + diffuse*Mcolour*Lcolour + Lcolour*specular, vec4(1.0));
	gl_FragColor.a = Mcolour.a;
}