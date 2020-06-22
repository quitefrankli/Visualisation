#include <iostream>
#include <chrono>
#include <thread>
#include <SDL2/SDL.h>

#include "Shader.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Window.hpp"

using namespace std;
using namespace chrono;
using namespace this_thread;

const int initialWindowWidth = 500;
const int initialWindowHeight = 250;

void viewMode(string fileName) {

	Window window("View Mode", initialWindowWidth, initialWindowHeight);

	std::string dir = "D:/Programming/Visual Studio/Visualisation/EXE/";
	Shader shader(dir + "VertexShader.glsl", dir + "FragmentShader.glsl");
	shader.bind();

	Texture texture(fileName);
	texture.bind();

	Rectangle rectangle;
	Mesh mesh(rectangle);

	window.resize(texture.width, texture.height);

	while (window.isOpen()) {
		std::string retVal = window.pollEvents();
		//if (retVal == "-") {
		//	rectangle.scale(0.5f, 0.5f);
		//	meshes.pop_back();
		//	meshes.emplace_back(rectangle);
		//} else if (retVal == "=") {
		//	rectangle.scale(2.0f, 2.0f);
		//	meshes.pop_back();
		//	meshes.emplace_back(rectangle);
		//} else if (retVal == "Left") {
		//	rectangle.translate(-0.25, 0.0f);
		//	meshes.pop_back();
		//	meshes.emplace_back(rectangle);
		//} else if (retVal == "Right") {
		//	rectangle.translate(0.25, 0.0f);
		//	meshes.pop_back();
		//	meshes.emplace_back(rectangle);
		//} else if (retVal == "Up") {
		//	rectangle.translate(0.0f, 0.25);
		//	meshes.pop_back();
		//	meshes.emplace_back(rectangle);
		//} else if (retVal == "Down") {
		//	rectangle.translate(0.0f, -0.25);
		//	meshes.pop_back();
		//	meshes.emplace_back(rectangle);
		//} else if (retVal == "R") {
		//	rectangle = Rectangle();
		//	meshes.pop_back();
		//	meshes.emplace_back(rectangle);
		//} else if (retVal == "D") {
		//	draw = !draw;
		//}

		window.clear();

		mesh.draw();

		window.swap();

		sleep_for(milliseconds(50));
	}
}
