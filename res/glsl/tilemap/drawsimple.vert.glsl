#version 330 core

uniform mat3 uProjection;
uniform mat3 uView;
uniform mat3 uModel;
uniform sampler2D uTileset;
uniform float uTilesize;

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec2 aBlendTexCoords;
layout (location = 3) in vec2 aOverlapTexCoords;

out vec2 vPosition;
out vec2 vTexCoords;
out vec2 vBlendTexCoords;
out vec2 vOverlapTexCoords;

void main() {
	vPosition = aPosition;
	vTexCoords = aTexCoords;
	vBlendTexCoords = aBlendTexCoords;
	vOverlapTexCoords = aOverlapTexCoords;
	
	gl_Position.xyz = uProjection * uView * uModel * vec3(aPosition, 1.0);
	gl_Position.w = 1.0;
}
