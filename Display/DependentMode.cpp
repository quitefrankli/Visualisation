#include <iostream>
#include <chrono>
#include <thread>
#include <SDL2/SDL.h>

#include <MyLibs/SharedMemory.hpp>

#include "Shader.hpp"
#include "Window.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"

using namespace std;
using namespace chrono;
using namespace this_thread;

void multistage(Window& window, SHM::VisualisationStruct* vis);

void dependentMode(string key) {
	SHM::VisualisationStruct* vis;

	SMObject o_vis(key, sizeof(SHM::VisualisationStruct));
	assert(o_vis.SMAccess());
	vis = (SHM::VisualisationStruct *)(o_vis.pData);
	Window window("Dependent Mode", SHM::initialWindowWidth, SHM::initialWindowHeight);

	std::string dir = "C:/Users/Nimda/programming/Visualisation/Display/";
	Shader shader(dir + "VertexShader.glsls", dir + "FragmentShader.glsls");
	shader.bind();

	//Rectangle rectangle;
	class Rectangle rectangle;
	Mesh mesh(rectangle);
	vis->ready = true;

	SDL_Event event;
	while (window.isOpen()) {
		window.pollEvents();

		if (vis->exit)
			window.close();

		if (vis->multistage)
			multistage(window, vis);
		else
			if (vis->flush) {
				window.resize(vis->width, vis->height);
				window.clear();

				Texture texture(vis->data, vis->height, vis->width, vis->colors, vis->padding);
				texture.bind();
				mesh.draw();
				window.swap();
				vis->flush = false;
			}

		sleep_for(milliseconds(5));
	}
}

void multistage(Window& window, SHM::VisualisationStruct* vis) {
	const int imageWidth = vis->width;
	const int imageHeight = vis->height;
	const std::pair<int, int> ratio = { vis->nAcross, vis->nDown };

	window.resize(imageWidth*ratio.first, imageHeight*ratio.second);

	class Rectangle originalRectangle;
	originalRectangle.scale(1.0f / float(ratio.first), 1.0f / float(ratio.second));
	originalRectangle.translate(-1.0f, 1.0f - 2.0f / float(ratio.second), true);

	window.clear();

	int row = 0;
	int col = 0;
	while (vis->multistage) {
		while (vis->flush == false)
			sleep_for(milliseconds(1));

		Texture texture(vis->data, vis->height, vis->width, vis->colors, vis->padding);

		class Rectangle rect = originalRectangle;
		rect.shift(col, -row);
		Mesh mesh(rect);

		mesh.draw();

		col++;
		if (col >= ratio.first) {
			row++;
			col = 0;
		}

		vis->flush = false;
	}

	window.swap();
	vis->multistage = false;
}