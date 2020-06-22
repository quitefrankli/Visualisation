#pragma once

#include <string>

#include <GL/glew.h>

class Texture {
public:
	Texture(const std::string& fileName);
	Texture(unsigned char imageData[], int height, int width, int colors, int padding);
	Texture(Texture&& src) : texture(src.texture) {}
	~Texture();
	
	int width, height, numComponents;
	std::string fileName;

	void loadData_fromRAM(unsigned char imageData[], int height, int width, int colors, int padding);
	void open(const std::string& fileName);
	void bind();
private:
	void setGLTextureParams();
	GLuint texture;
};

