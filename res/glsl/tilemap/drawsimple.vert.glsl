#version 330 core

uniform mat3 uProjection;
uniform mat3 uView;

layout (location = 0) in vec2 aPosition;

out vec2 vPosition;

void main() {
	vPosition = aPosition;
	
	gl_Position.xyz = uProjection * uView * vec3(aPosition, 1.0);
	gl_Position.w = 1.0;
}
