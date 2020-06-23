#pragma once

#include <string>
#include <memory>

namespace SHM {
	const static int MAX_HEIGHT = 1050;
	const static int MAX_WIDTH = 1980;
	const static int MAX_COLORS = 4;
	const static int initialWindowHeight = 512;
	const static int initialWindowWidth = 512;

	//
	// Only struct necessary for 1-way communication and drawing stuff
	//

	struct VisualisationStruct {
		uint8_t data[MAX_HEIGHT*MAX_WIDTH*MAX_COLORS];
		int windowHeight = initialWindowHeight;
		int windowWidth = initialWindowWidth;
		int nAcross, nDown;
		int height = 1;
		int width = 1;
		int colors = 4;
		int padding = 1;
		
		bool ready = false; // will become true once startup is complete
		bool flush = false;
		bool drawPixel = true;
		bool exit = false;
		bool verticallyFlipped = false;
		bool multistage = false;
	};

	void launchEXE(std::string fileName);
}

class SMObject {
public:
	SMObject(std::string szname, int size);
	~SMObject();

	bool SMCreate();
	bool SMAccess();

	void* pData = nullptr;
private:
	struct PIMPL; // stands for pointer to implementation in order to not have to #include windows.h in header file
	std::unique_ptr<PIMPL> pimpl;
	int size;
};
