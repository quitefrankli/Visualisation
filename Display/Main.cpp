//
// The main purpose of this program is to serve as a fast interprocess data visualiser
// Display can also be built in debug mode and enable a console
// Because Display relies on OpenGL, it can only be built as a 32 bit executable
// However it's possible for 64 bit processes to communicate with it
//

#include <Windows.h>

#include <sstream>
#include <vector>

void standaloneMode();
void dependentMode(std::string key);
void viewMode(std::string fileName);

#ifdef _DEBUG // adds console to check outputs
int main(int argc, char* argv[]) {
	std::vector<std::string> cmdLine(argv + 1, argv + argc);
#else
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	// can only really assume right number of command line arguments
	std::wstringstream wss(pCmdLine);
	std::wstring wstr;
	std::vector<std::string> cmdLine;
	while (wss >> wstr)
		cmdLine.emplace_back(std::string(wstr.begin(), wstr.end())); // this truncates the unicode
#endif
	if (!(cmdLine.size() >= 0 && cmdLine.size() <= 2))
		throw std::exception("Incorrect number of command line arguments");

	if (cmdLine.empty())
		standaloneMode();

	// dependent mode is the main use of this program, other modes are for debugging only
	else if (cmdLine[0] == "dependent") 
		dependentMode(cmdLine[1]);
	else
		viewMode(cmdLine[0]);

	return 0;
}
