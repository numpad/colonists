#version 330 core

uniform mat3 uProjection;
uniform mat3 uView;
uniform sampler2D uTileset;
uniform float uTilesize;

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aTexCoords;

out vec2 vPosition;
out vec2 vTexCoords;

void main() {
	vPosition = aPosition;
	vTexCoords = aTexCoords;
	
	gl_Position.xyz = uProjection * uView * vec3(aPosition, 1.0);
	gl_Position.w = 1.0;
}
