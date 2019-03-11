#version 330 core

uniform sampler2D uTileset;
uniform float uTilesize;

in vec2 vPosition;
in vec2 vTexCoords;
in vec2 vBlendTexCoords;
in vec2 vOverlapTexCoords;

out vec4 FragColor;

void main() {
	vec4 tile = texture(uTileset, vTexCoords);
	vec4 blend = texture(uTileset, vBlendTexCoords);
	vec4 overlap = texture(uTileset, vOverlapTexCoords);
	
	float p = blend.a;
	
	FragColor.rgb = mix(tile.rgb, overlap.rgb, p);
	FragColor.a = tile.a;
}
