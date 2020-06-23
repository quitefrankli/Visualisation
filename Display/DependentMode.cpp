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

//
// Main purpose of this program, which is to visualise data sent by other processes
//

void dependentMode(string key) {
	SHM::VisualisationStruct* vis;

	// Shared memory
	SMObject o_vis(key, sizeof(SHM::VisualisationStruct));
	if (!o_vis.SMAccess())
		throw std::exception("Could not access shared memory");

	vis = (SHM::VisualisationStruct *)(o_vis.pData);
	Window window("Dependent Mode", SHM::initialWindowWidth, SHM::initialWindowHeight);

	// Shaders, at the moment it's compiled at runtime, but in the future 
	// it would be best if it was compiled at compile time
	Shader shader;
	shader.bind();

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
			// flush acts as a quick means of locking
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

// this mode allows for graphic data to be streamed one chunk at a time
// instead of instantaneous, very useful for visualising collection of related data
void multistage(Window& window, SHM::VisualisationStruct* vis) {
	const int imageWidth = vis->width;
	const int imageHeight = vis->height;
	const std::pair<int, int> ratio = { vis->nAcross, vis->nDown };

	window.resize(imageWidth*ratio.first, imageHeight*ratio.second);

	class Rectangle originalRectangle;
	originalRectangle.scale(1.0f / float(ratio.first), 1.0f / float(ratio.second));
	originalRectangle.translate(-1.0f, 1.0f - 2.0f / float(ratio.second), true);

	window.clear();

	for (int row = 0, col = 0; vis->multistage;) {
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