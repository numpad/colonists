#version 330 core

uniform sampler2D uTileset;
uniform float uTilesize;

in vec2 vPosition;
in vec2 vTexCoords;

out vec4 FragColor;

void main() {
	const float pixel = 1.0 / 960.0;
	const float tile = 96.0 / 960.0;
	
	vec2 texCoords = tile * mod((vPosition) / uTilesize, 1.0);
	
	FragColor = vec4(vTexCoords.xy * 10.0, 0.0, 1.0);
	//FragColor = texture(uTileset, vTexCoords);
}
