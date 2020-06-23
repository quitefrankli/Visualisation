#pragma once

#include <vector>
#include <string>

#include "SharedMemory.hpp"

namespace VIS {
	static const size_t MAX_COORDINATES = 3;
	static const size_t MAX_COLORS = 4;

	struct Pixel {
		float position[MAX_COORDINATES]; // x,y,z
		uint8_t color[MAX_COLORS];
	};

	class Visualisation {
	public:
		Visualisation();
		~Visualisation();

		SHM::VisualisationStruct* vis = nullptr; // can't be unique ptr

		void draw(const uint8_t* data, size_t width, size_t height, size_t colors, bool verticalFlip, bool swapRedBlue, int padding);
		void draw(const uint8_t* data, size_t width, size_t height);

		inline bool isReady() { 
			if (vis == nullptr)
				throw "Error: VIS is nullptr"; 
			return vis->ready; 
		}
		inline void close() { vis->exit = true; }
	private:
		std::string tempString;
		SMObject sm;
		void drawProperties(size_t width, size_t height);
	};

	class Multistage : public Visualisation {
	public:
		Multistage(int nAcross, int nDown, int width, int height, int components = 3);

		void sendData(const uint8_t* data);

		// implicitly signals end of multistage
		void draw();
	};
}
