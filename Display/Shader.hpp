#pragma once

#include <string>
#include <GL/glew.h>

class Shader {
public:
	Shader(const std::string& vertexShader, const std::string& fragmentShader);
	~Shader();
	void bind();

private:
	static const unsigned int NUM_SHADERS = 2;

	// keeps track of program shaders and other glew stuff
	GLuint program;
	GLuint shaders[NUM_SHADERS];
};