#include <iostream>

#include "Mesh.hpp"

std::vector<GLuint> Mesh::vertexArrayObjects;

Vertex::Vertex(const glm::vec3& pos, const glm::vec2& texCoord) {
	this->pos = pos;
	this->texCoord = texCoord;
}

Rectangle::Rectangle() {
	vertices = {
		Vertex(glm::vec3(-1.0, -1.0, 0.0), glm::vec2(0.0, 1.0)),
		Vertex(glm::vec3(-1.0,  1.0, 0.0), glm::vec2(0.0, 0.0)),
		Vertex(glm::vec3(1.0,  1.0, 0.0), glm::vec2(1.0, 0.0)),
		Vertex(glm::vec3(1.0,  1.0, 0.0), glm::vec2(1.0, 0.0)),
		Vertex(glm::vec3(1.0, -1.0, 0.0), glm::vec2(1.0, 1.0)),
		Vertex(glm::vec3(-1.0, -1.0, 0.0), glm::vec2(0.0, 1.0)),
	};
}

void Rectangle::scale(float x, float y) {
	for (auto& vertex : vertices) {
		vertex.pos.x *= x;
		vertex.pos.y *= y;
	}
}

void Rectangle::translate(float x, float y, bool absolute) {
	if (absolute) {
		float width = abs(vertices[0].pos.x - vertices[2].pos.x);
		float height = abs(vertices[0].pos.y - vertices[1].pos.y);
		vertices[0].pos = glm::vec3(x, y, vertices[0].pos.z);
		vertices[1].pos = glm::vec3(x, y + height, vertices[1].pos.z);
		vertices[2].pos = glm::vec3(x + width, y + height, vertices[2].pos.z);
		vertices[3].pos = vertices[2].pos;
		vertices[4].pos = glm::vec3(x + width, y, vertices[4].pos.z);
		vertices[5].pos = vertices[0].pos;
	} else {
		for (auto& vertex : vertices) {
			vertex.pos.x += x;
			vertex.pos.y += y;
		}
	}
}

void Rectangle::shift(float x, float y) {
	float width = abs(vertices[0].pos.x - vertices[2].pos.x);
	float height = abs(vertices[0].pos.y - vertices[1].pos.y);
	translate(x * width, y * height);
}

Mesh::Mesh(std::vector<Vertex> vertices) {
	drawCount = vertices.size();
	curVertexArrayObjectIndex = vertexArrayObjects.size();
	GLuint vertexObject;
	glGenVertexArrays(1, &vertexObject);
	glBindVertexArray(vertexObject);
	vertexArrayObjects.push_back(vertexObject);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;

	for (auto& vertex : vertices) {
		positions.push_back(vertex.pos);
		texCoords.push_back(vertex.texCoord);
	}

	glGenBuffers(NUM_BUFFERS, vertexArrayBuffers);

	//
	// Vertex Position
	//

	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[POSITION_VB]);	
	// takes data from ram and puts into gpu memory
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(positions[0]), positions.data(), GL_STATIC_DRAW); // static draw means data wont be expected to be changed
	// tell gpu how to interpret the data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);	

	//
	// Texture Position
	//

	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(texCoords[0]), texCoords.data(), GL_STATIC_DRAW); // static draw means data wont be expected to be changed
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &vertexArrayObjects[curVertexArrayObjectIndex]);
	glDeleteVertexArrays(NUM_BUFFERS, vertexArrayBuffers);
}

void Mesh::draw() {
	// not really sure if bottom two comments are needed
	glBindVertexArray(vertexArrayObjects[curVertexArrayObjectIndex]);
	//glBindVertexArray(vertexArrayBuffers[0]);
	glDrawArrays(GL_TRIANGLES, 0, drawCount);
	//glBindVertexArray(0);
}

