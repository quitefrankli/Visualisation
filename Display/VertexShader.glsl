#version 430

in layout(location = 0) vec3 position;
in layout(location = 1) vec2 texCoord;

out vec2 texCoordinates;

void main() {
	gl_Position = vec4(position, 1.0);
	texCoordinates = texCoord;
}