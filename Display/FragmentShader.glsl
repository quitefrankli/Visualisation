#version 430

// uniform means both cpu and gpu can read from it
uniform sampler2D diffuse;

in vec2 texCoordinates;

out vec4 fragColor;

void main() {
	fragColor = texture2D(diffuse, texCoordinates);
}