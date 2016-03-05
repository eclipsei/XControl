
#ifndef XINPUTWRAPPER_
#define XINPUTWRAPPER_

#include <Windows.h>
#include <iostream>
#include <vector>

#include <Xinput.h>
#include "VirtualInput.h"

#define MAP_TYPE_MOUSE 0x01
#define MAP_TYPE_KEYBOARD 0x02

class XInputWrapper {

private:

	int controllerID; // ID for the controller (if multiple controllers this is currently only the first one)
	float deadzoneX_LT = 0.1f; // deadzone for the left analog thumbsticks in x direction (use positive values)
	float deadzoneY_LT = 0.1f; // deadzone for the left analog thumbsticks in y direction (use positive values)
	float deadzoneX_RT = 0.1f; // deadzone for right analog stick
	float deadzoneY_RT = 0.12f; // deadzone for right analog stick

	typedef struct ThumbStates { // struct for the thumbstick states, values ranging from -1 to 1 (left to right, down to up)
		float LTx;
		float LTy;
		float RTx;
		float RTy;
	};
	
	ThumbStates thumbSticks;
	XINPUT_STATE currentState;
	XINPUT_STATE previousState;

	void UpdateScene(); // function containing the scene updating actions (such as moving the mouse and clicking etc.)
	bool IsPressed(WORD, XINPUT_STATE); // check if a button is pressed in a state

	typedef struct MappedKey {
		int type;
		WORD controllerButton;
		WORD virtualKey;
	};
	std::vector<MappedKey> mapConfiguration; // holds the mapped keys

public:

	XINPUT_GAMEPAD* GetCurrentState(); // gets the raw controller state
	XINPUT_GAMEPAD* GetPreviousState();
	bool CheckConnection(); // checks if a controller is connected (sets controllerID value to the first controller ID)
	int GetControllerID(); // returns the controller ID
	bool Refresh(); // returns false if controller is disconnected, otherwise refreshes the state variables and updates based on inputs
	// this has to be called every time you want to update based on controller input (so basically every frame)
	void Start(int); // simply loops the refresh function with a specified refresh rate, use for example in a new thread

	void InitConfiguration(); // maps default values for controller to keyboard/mouse input
};


#endif