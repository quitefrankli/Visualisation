//
// The main purpose of this program is to serve as a fast interprocess data visualiser
// Display can also be built in debug mode and enable a console
// Because Display relies on OpenGL, it can only be built as a 32 bit executable
// However it's possible for 64 bit processes to communicate with it
//

#include <Windows.h>

#include <sstream>
#include <vector>
#include <iostream>

void standaloneMode();
void dependentMode(std::string key);
void viewMode(std::string fileName);

int main(int argc, char* argv[]) {
	std::vector<std::string> cmdLine(argv + 1, argv + argc);
	if (!(cmdLine.size() >= 0 && cmdLine.size() <= 2))
		throw std::exception("Incorrect number of command line arguments");

	try
	{
		if (cmdLine.empty())
			standaloneMode();

		// dependent mode is the main use of this program, other modes are for debugging only
		else if (cmdLine[0] == "dependent") 
			dependentMode(cmdLine[1]);
		else
			viewMode(cmdLine[0]);
	} catch (const std::exception& err)
	{
		std::cerr<<"ERROR: "<<err.what()<<std::endl;
	} catch (...)
	{
		std::cerr<<"Unknown Exception Caught!"<<std::endl;
		throw;
	}
}
