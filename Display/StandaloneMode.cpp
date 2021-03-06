//
// Only useful for debugging purposes
//

//
// Default standalone mode
//

#include <iostream>
#include <chrono>
#include <thread>
#include <filesystem>
#include <list>

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

const std::string textureDirectory = "Place your directory here";

void standaloneMode() {
	Window window("Standalone Mode", initialWindowWidth, initialWindowHeight);

	Shader shader;
	shader.bind();

	srand(time(NULL));

	std::list<Texture> textures;
	if (!std::filesystem::exists(textureDirectory))
		throw std::exception("Bad texture directory");
	for (auto& entry : std::filesystem::directory_iterator(textureDirectory))
		textures.emplace_back(entry.path().string());

	// for each texture give 100x100px and resize window
	const int imageWidth = 300;
	const int imageHeight = 300;
	const std::pair<int, int> originalRatio = { 5, 2 };
	std::pair<int, int> ratio = originalRatio;
	for (int multiplier = 1; (ratio.first * ratio.second)*multiplier < textures.size(); ratio.first += originalRatio.first, ratio.second += originalRatio.second);
	
	window.resize(imageWidth*ratio.first, imageHeight*ratio.second);

	Rectangle rectangle;
	rectangle.scale(1.0f/float(ratio.first), 1.0f/float(ratio.second));
	rectangle.translate(-1.0f, -1.0f, true);

	std::list<Mesh> meshes;
	{
		auto it_texture = textures.begin();
		for (int row = 0; it_texture != textures.end(); row++) {
			for (int col = 0; it_texture != textures.end() && col < ratio.first; col++, it_texture++) {
				Rectangle rect = rectangle;
				rect.shift(col, row);
				meshes.emplace_back(rect);
			}
		}
	}
		
	bool draw = true;
	while (window.isOpen()) {
		std::string retVal = window.pollEvents();

		window.clear();

		if (draw) {
			auto it_texture = textures.begin();
			auto it_mesh = meshes.begin();

			while (it_texture != textures.end()) {
				it_texture->bind();
				it_mesh->draw();
				it_texture++;
				it_mesh++;
			}
		}

		window.swap();

		sleep_for(milliseconds(50));
	}
}
