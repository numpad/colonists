#version 330 core

uniform sampler2D uTileset;
uniform float uTilesize;

in vec2 vPosition;
in vec2 vTexCoords;

out vec4 FragColor;

void main() {
	vec2 id = ceil(vPosition / uTilesize);
	
	vec2 texCoords = (96.0 / 960.0f) * (mod((vPosition + 1.0 / 960.0) / uTilesize, 1.0));
	texCoords = clamp(texCoords, 0., 1.);
	FragColor = texture(uTileset, texCoords);
	
	if (int(ceil(id.x)) % 2 == int(ceil(id.y)) % 2) {
		FragColor.rgb = FragColor.brg;
	}
}
