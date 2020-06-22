#include <iostream>

#include <GL/glew.h>

#include "Window.hpp"

Window::Window(std::string name, size_t height, size_t width) {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);		// 8 is convention i.e. 1 byte
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);	// amount of data SDL will allocate for every pixel
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);	// allocate space for another 'window', so a duplicate of buffer size, double buffering for swap

	window = SDL_CreateWindow(
		name.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		height,
		width,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);
	
	if (window == nullptr) return;
	context = SDL_GL_CreateContext(window);
	if (glewInit() != GLEW_OK)
		throw std::string("Error: MyWindow");

	this->height = height;
	this->width = width;

	open = true;
}

Window::~Window() {
	SDL_GL_DeleteContext(context);
	if (window != nullptr) SDL_DestroyWindow(window);
	SDL_Quit();
}

std::string Window::pollEvents() {
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			open = false;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				open = false;
				break;
			default:
				return SDL_GetKeyName(event.key.keysym.sym);
			}
		}
	}

	return "nothing";
}

void Window::clear() {
	glClearColor(0.0f, 0.15f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::swap() {
	SDL_GL_SwapWindow(window);
}

void Window::resize(int width, int height) {
	SDL_SetWindowSize(window, width, height);
	glViewport(0, 0, width, height);
}