
#include "System.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	//initialize the program
	std::unique_ptr<System> system(new System);
	system->Init();
	system->Run();
	system->Shutdown();
	return 0;
}