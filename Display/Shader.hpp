#pragma once

#include <string>
#include <GL/glew.h>

// bit of a hack to read file during compile time, TODO: compile shaders during compile time
// https://stackoverflow.com/questions/20443560/how-to-practically-ship-glsl-shaders-with-your-c-software
// see Jan Ruegg's response 

const std::string vertexShader =
#include "Shaders/VertexShader.glsl"
"";

const std::string fragmentShader =
#include "Shaders/FragmentSHader.glsl"
"";

class Shader {
public:
	Shader();
	~Shader();
	void bind();

private:
	static const unsigned int NUM_SHADERS = 2;

	// keeps track of program shaders and other glew stuff
	GLuint program;
	GLuint shaders[NUM_SHADERS];
};