#version 330 core

uniform sampler2D uTileset;
uniform float uTilesize;

in vec2 vPosition;
in vec2 vTexCoords;

out vec4 FragColor;

void main() {
	FragColor = texture(uTileset, vTexCoords);
}
