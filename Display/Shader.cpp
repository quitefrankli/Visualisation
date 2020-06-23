#include <fstream>
#include <streambuf>
#include <iostream>
#include <sstream>

#include "Shader.hpp"

//
// Helper Functions
//

static void checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage) {
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram) 
		glGetProgramiv(shader, flag, &success);
	else 
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE) {
		if (isProgram) 
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else 
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		std::cerr << errorMessage << ": '" << error << "'" << std::endl;

		throw std::exception(errorMessage.c_str());
	}
}

static GLuint createShader(std::string text, GLenum shaderType) {
	GLuint shader = glCreateShader(shaderType);

	// build and compile shader text
	if (shader == 0)
		throw std::exception("Could not create shader");

	const GLchar* shaderSourceStrings[1];
	GLint shaderSourceStringLengths[1];
	shaderSourceStrings[0] = text.c_str();
	shaderSourceStringLengths[0] = text.length();

	glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths);
	glCompileShader(shader);

	checkShaderError(shader, GL_COMPILE_STATUS, false, "Shader: Shader compilation failed");

	return shader;
}

//
// Memeber Functions
//

Shader::Shader() {
	shaders[0] = createShader(vertexShader, GL_VERTEX_SHADER);
	shaders[1] = createShader(fragmentShader, GL_FRAGMENT_SHADER);

	program = glCreateProgram();
	for (size_t i = 0; i < NUM_SHADERS; i++)
		glAttachShader(program, shaders[i]);

	// linking shaders
	glLinkProgram(program);
	checkShaderError(program, GL_LINK_STATUS, true, "Shader: Program linking failed");

	// validating shader programs
	glValidateProgram(program);
	checkShaderError(program, GL_VALIDATE_STATUS, true, "Shaders: Program is invalid");
}

Shader::~Shader() {
	for (size_t i = 0; i < NUM_SHADERS; i++) {
		glDetachShader(program, shaders[i]);
		glDeleteShader(shaders[i]);
	}
	glDeleteProgram(program);
}

void Shader::bind() {
	glUseProgram(program);
}
