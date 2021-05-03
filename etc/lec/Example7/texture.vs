/*
 *  Simple vertex shader for the first
 *  texture example.
 */
#version 330 core

in vec4 vPosition;
in vec2 vTexture;

uniform mat4 modelView;

out vec2 texCoord;

float angle = 4.50;
mat2 rotationMatrix = mat2(cos(angle), sin(angle), -sin(angle), cos(angle));

void main(void) {
	gl_Position = modelView * vPosition;
	texCoord = vTexture * rotationMatrix;
}