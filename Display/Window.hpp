#pragma once

#include <string>

#include <SDL2/SDL.h>

class Window {
public:
	Window(std::string name, size_t height, size_t width);
	~Window();
	inline bool isOpen() { return open; }
	inline void close() { open = false; }
	inline SDL_Window* getWindow() { return window; }
	std::string pollEvents();

	void clear();
	void swap();
	void resize(int width, int height);
private:
	SDL_Window* window = nullptr;
	SDL_GLContext context;
	SDL_Event event;
	bool open = false;
	size_t height;
	size_t width;
};