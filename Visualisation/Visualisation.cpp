/* Important Note
	always compile static library first before the exe if changes were made to visualisation code
	since opengl only runs on 32 bits, compile library for 32 bit first then build the opengl executable then recompile
		static library for 64 bit so that other programs can also be compiled on 64 bit instead of 32 bits
*/

//
// Visualisation is intended to be compiled as a static library so that
// fast interprocess graphic visualisation can be achieved without long compile times
//

#include <iostream>
#include <chrono>
#include <thread>
#include <SimpleIni.h>
#include <string>

#include "Visualisation.hpp"
#include "Maths.hpp"

using VIS::Visualisation;

//
// helper functions
//

// 10s timeout
static void waitForExpected(bool& actual, bool expected) {
	auto beg = std::chrono::system_clock::now();
	auto timeout = std::chrono::duration<int>(10);
	while ((std::chrono::system_clock::now() - beg) < timeout) {
		if (actual == expected)
			return;
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	throw std::exception("Error in VISUALISATION LIBRARY: Time Out Error");
}

// responsible for generating the 'key' to be given to other processes to access a shared memory space
static std::string RandomString(int len) {
	std::string randomString;
	for (int i = 0; i < len; i++)
		randomString += Math::RandomUniform(int('A'), int('Z'));
	return randomString;
}

// 
// member functions
//

Visualisation::Visualisation() :
	tempString(RandomString(10)),
	sm(tempString, sizeof(SHM::VisualisationStruct)) 
{
	if (!sm.SMCreate() || !sm.SMAccess())
		throw std::exception("Visualisation initialisation failed");

	std::cout << "Visualisation Initialisation success, launching visualiser..." << std::endl;
	vis = (SHM::VisualisationStruct*)sm.pData;
	vis->ready = false;

	// relies on user to add the path Visualisation.exe to config.ini
	CSimpleIniA config;
	config.LoadFile("VisualisationConfig.ini");
#ifdef _DEBUG
	std::string path = config.GetValue("", "pathDebug");
#else
	std::string path = config.GetValue("", "path");
#endif
	SHM::launchEXE(path + " dependent " + tempString);

	waitForExpected(vis->ready, true);
}

Visualisation::~Visualisation() {
	if (vis != nullptr) 
		vis->exit = true;
}

void Visualisation::drawProperties(size_t width, size_t height) {
	vis->width = width;
	vis->height = height;
	vis->flush = true;
}

void VIS::Visualisation::draw(const uint8_t* data, size_t width, size_t height, size_t colors, bool verticalFlip, bool swapRedBlue, int padding) {
	waitForExpected(vis->flush, false);

	int rowLen = width * colors;
	int remainder = rowLen % padding;
	int extraPaddingNeeded = remainder == 0 ? 0 : padding - remainder;
	int trueRowLen = rowLen + extraPaddingNeeded;
	int area = trueRowLen * height;

	memcpy(vis->data, data, area);

	if (swapRedBlue)
		for (int row = 0; row < height; row++)
			for (int col = 2; col < rowLen; col+=colors)
				std::swap(vis->data[row * trueRowLen + col], vis->data[row * trueRowLen + col - 2]);

	vis->colors = colors;
	vis->padding = padding;
	vis->verticallyFlipped = verticalFlip;
	drawProperties(width, height);
}

// draw under naive approach (ie. 3 colors RGB, no padding, no vertical mirror, no red-blue swaps)
void VIS::Visualisation::draw(const uint8_t* data, size_t width, size_t height) {
	memcpy(vis->data, data, width * height * 3);
	vis->colors = 3;
	vis->padding = 1; // synonymous with no padding
	vis->verticallyFlipped = false;
	drawProperties(width, height);
}
	
VIS::Multistage::Multistage(int nAcross, int nDown, int width, int height, int components) {
	vis->nAcross = nAcross;
	vis->nDown = nDown;
	vis->width = width;
	vis->height = height;
	vis->colors = components;
	vis->padding = 1;
	vis->verticallyFlipped = false;
	vis->multistage = true;
}

void VIS::Multistage::sendData(const uint8_t* data) {
	waitForExpected(vis->flush, false);

	memcpy(vis->data, data, vis->width*vis->height*vis->colors);
	vis->flush = true;
}

void VIS::Multistage::draw() {
	vis->multistage = false;
}
