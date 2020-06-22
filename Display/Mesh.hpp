#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

class Vertex {
public:
	Vertex(const glm::vec3& pos, const glm::vec2& texCoord);
	glm::vec3 pos;
	glm::vec2 texCoord;
};

class Rectangle {
public:
	Rectangle();
	Rectangle& operator=(const Rectangle& rectangle) {
		this->vertices = rectangle.vertices;
		return *this;
	}

	std::vector<Vertex> vertices;
	void scale(float x, float y);
	void translate(float x, float y, bool absolute = false);
	
	// similar to translate but moves relative to size of rectangle
	void shift(float x, float y);
};

class Mesh {
public:
	Mesh(std::vector<Vertex> vertices);
	Mesh(class Rectangle rectangle) : Mesh(rectangle.vertices) {}
	~Mesh();

	void draw();
private:
	enum {
		POSITION_VB,
		TEXCOORD_VB,
		NUM_BUFFERS
	};
	
	size_t curVertexArrayObjectIndex;
	static std::vector<GLuint> vertexArrayObjects;
	GLuint vertexArrayBuffers[NUM_BUFFERS];

	size_t drawCount;
};