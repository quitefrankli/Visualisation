#include "Texture.hpp"

#include <fstream>
#include <cassert>

#define STB_IMAGE_IMPLEMENTATION // needed for stbi
#include <stb_image.h>

// static function for gl stuff

void Texture::setGLTextureParams() {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// s & t for x & y very weird naming convention
	// wraps textures around those aforementioned axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture::Texture(const std::string& fileName) {
	open(fileName);
}

Texture::Texture(unsigned char imageData[], int height, int width, int colors, int padding) {
	loadData_fromRAM(imageData, height, width, colors, padding);
}

Texture::~Texture() {
	glDeleteTextures(1, &texture);
}

void Texture::loadData_fromRAM(unsigned char imageData[], int height, int width, int colors, int padding) {
	assert(imageData != nullptr);
	setGLTextureParams();

	GLint internalFormat;
	GLenum format;
	switch (colors) {
	case 3:
		internalFormat = format = GL_RGB;
		break;
	case 4:
		internalFormat = format = GL_RGBA;
		break;
	default:
		throw std::exception("BAD COLOR");
		break;
	}

	// alignment issues here
	if (padding != 1 && padding != 4)
		throw std::exception("BAD PADDING");
	glPixelStorei(GL_UNPACK_ALIGNMENT, padding);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
}

void Texture::open(const std::string& fileName) {
	bool useSTB = true;
	stbi_uc* imageData = stbi_load(fileName.c_str(), &width, &height, &numComponents, 4);

	// sometimes it's convenient to store bitmap image without all the standard headers
	// so it's nice to be able interpret a bitmap that only has the dimensions as the header
	if (imageData == nullptr) {
		std::ifstream imageFile(fileName, std::ios::in | std::ios::binary);

		imageFile >> width;
		imageFile >> height;
		numComponents = 3;
		imageFile.seekg(1, std::ios::cur); // get rid of new line

		int numBytes = width * height * numComponents;
		auto actualData = std::make_unique<char[]>(numBytes);
		imageFile.read(actualData.get(), numBytes);
		
		loadData_fromRAM((stbi_uc*)actualData.get(), height, width, numComponents, 1);

		return;
	}

	this->fileName = fileName;
	setGLTextureParams();

	// for resolution (min and magnification), GL_LINEAR for linear extrapolation
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// send texture to gpu
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	stbi_image_free(imageData);
}

void Texture::bind() {
	glActiveTexture(GL_TEXTURE0); // allows for a bunch of different active texture
	glBindTexture(GL_TEXTURE_2D, texture);
}
