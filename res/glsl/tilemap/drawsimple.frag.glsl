#version 330 core

uniform sampler2D uTileset;

in vec2 vPosition;
out vec4 FragColor;

void main() {
	vec2 id = ceil(vPosition / 128.0);
	
	if (int(id.x) % 2 == int(id.y) % 2)
		FragColor = vec4(0.4, 0.8, 0.2, 1.0);
	else
		FragColor = vec4(0.2, 0.7, 0.1, 1.0);
}
