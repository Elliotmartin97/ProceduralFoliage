
#include "System.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	// Create the system object to initialize windows.
	std::unique_ptr<System> system(new System);
	// Run the system.
	bool result = system->Init();
	if (result)
	{
		system->Run();
	}
	system->ShutdownWindows();
	return 0;
}