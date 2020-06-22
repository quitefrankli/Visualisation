#include <fstream>
#include <streambuf>
#include <iostream>
#include <sstream>

#include "Shader.hpp"

//
// Helper Functions
//

static std::string loadShader(const std::string& fileName) {
	std::ifstream file(fileName);
	if (!file.is_open())
		throw std::exception("Could not load shader");

	std::stringstream buffer;
	buffer << file.rdbuf();

	return buffer.str();
}

static void checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage) {
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram) glGetProgramiv(shader, flag, &success);
	else glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE) {
		if (isProgram) glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}

static GLuint createShader(const std::string& text, GLenum shaderType) {
	GLuint shader = glCreateShader(shaderType);

	// build and compile shader text
	_ASSERT(shader != 0);

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

Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader) {
	shaders[0] = createShader(loadShader(vertexShader), GL_VERTEX_SHADER);
	shaders[1] = createShader(loadShader(fragmentShader), GL_FRAGMENT_SHADER);

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
