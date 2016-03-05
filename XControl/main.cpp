
#include <Windows.h>
#include <iostream>

#include <xinput.h>
#include "XInputWrapper.h"

int main(void) {

	XInputWrapper xiw = XInputWrapper(); // a handle for the wrapper
	xiw.InitConfiguration();

	if (xiw.CheckConnection()) { // check if controller found
		std::cout << "Controller found with ID: " << xiw.GetControllerID() << std::endl;
	}
	else {
		std::cout << "Controller was not found. Exiting...\n";
		system("PAUSE");
		return 1;
	}

	int refreshRate = 60;
	float sleepTime = 1000 / refreshRate;

	while (xiw.Refresh()) { // start the loop
		Sleep(sleepTime);
	}

	system("PAUSE");

	return 0;
}