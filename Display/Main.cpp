#include <Windows.h>

#include <sstream>
#include <vector>
#include <cassert>

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
	assert(cmdLine.size() >= 0 && cmdLine.size() <= 2);

	if (cmdLine.empty())
		standaloneMode();
	else if (cmdLine[0] == "dependent")
		dependentMode(cmdLine[1]);
	else
		viewMode(cmdLine[0]);

	return 0;
}
