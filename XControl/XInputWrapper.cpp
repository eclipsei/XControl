
#include "XInputWrapper.h"

/* Private functions */

void XInputWrapper::UpdateScene() {

	VirtualInput vi = VirtualInput();

	/* A button */
	if (IsPressed(XINPUT_GAMEPAD_A, currentState)) { // if pressed in the current state
		if (!IsPressed(XINPUT_GAMEPAD_A, previousState)) { // if not pressed in the previous state
			std::cout << "A pressed\n";
			vi.MouseLeftDown();
		}
	}
	else { // if not pressed in the current state
		if (IsPressed(XINPUT_GAMEPAD_A, previousState)) { // if pressed in the previous state
			vi.MouseLeftUp();
			std::cout << "A released\n";
		}
	}

	/* B button */
	if (IsPressed(XINPUT_GAMEPAD_B, currentState)) { // if pressed in the current state
		if (!IsPressed(XINPUT_GAMEPAD_B, previousState)) { // if not pressed in the previous state
			vi.MouseRightDown();
			std::cout << "B pressed\n";
		}
	}
	else { // if not pressed in the current state
		if (IsPressed(XINPUT_GAMEPAD_B, previousState)) { // if pressed in the previous state
			vi.MouseRightUp();
			std::cout << "B released\n";
		}
	}

	/* Y button */
	if (IsPressed(XINPUT_GAMEPAD_Y, currentState)) { // if pressed in the current state
		if (!IsPressed(XINPUT_GAMEPAD_Y, previousState)) { // if not pressed in the previous state
			vi.ScrollWheelDown();
			std::cout << "Y pressed\n";
		}
	}
	else { // if not pressed in the current state
		if (IsPressed(XINPUT_GAMEPAD_Y, previousState)) { // if pressed in the previous state
			vi.ScrollWheelUp();
			std::cout << "Y released\n";
		}
	}

	/* Left shoulder */
	if (IsPressed(XINPUT_GAMEPAD_LEFT_SHOULDER, currentState)) {
		if (!IsPressed(XINPUT_GAMEPAD_LEFT_SHOULDER, previousState)) {
			vi.MouseX1Down();
			std::cout << "Left shoulder pressed\n";
		}
	}
	else {
		if (IsPressed(XINPUT_GAMEPAD_LEFT_SHOULDER, previousState)) {
			vi.MouseX1Up();
			std::cout << "Left shoulder released\n";
		}
	}

	/* Right shoulder */
	if (IsPressed(XINPUT_GAMEPAD_RIGHT_SHOULDER, currentState)) {
		if (!IsPressed(XINPUT_GAMEPAD_RIGHT_SHOULDER, previousState)) {
			vi.MouseX2Down();
			std::cout << "Right shoulder pressed\n";
		}
	}
	else {
		if (IsPressed(XINPUT_GAMEPAD_RIGHT_SHOULDER, previousState)) {
			vi.MouseX2Up();
			std::cout << "Right shoulder released\n";
		}
	}

	/* Moving the mouse based on left thumbstick */
	vi.MoveMouse(thumbSticks.LTx, thumbSticks.LTy);

	/* Scrolling based on the right thumbstick */
	vi.ScrollWheelMove(thumbSticks.RTy);

}

bool XInputWrapper::IsPressed(WORD button, XINPUT_STATE state) {
	return (state.Gamepad.wButtons & button) != 0;
}

/* Public functions */

XINPUT_GAMEPAD* XInputWrapper::GetCurrentState() {
	return &currentState.Gamepad;
}

bool XInputWrapper::CheckConnection() {
	controllerID = -1;
	for (int i = 0; i < XUSER_MAX_COUNT; i++) {
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));
		if (XInputGetState(i, &state) == ERROR_SUCCESS) { // if succesfully retrieved controller state
			controllerID = i;
			return true; // return true when first controller succesfully found
		}
	}
	return false; // return false when no controller found
}

int XInputWrapper::GetControllerID() {
	return controllerID;
}

bool XInputWrapper::Refresh() {

	previousState = currentState; // storing the state before updating

	if (controllerID = -1) {
		CheckConnection();
	}
	if (controllerID != -1) {
		ZeroMemory(&currentState, sizeof(XINPUT_STATE));
		if (XInputGetState(controllerID, &currentState) != ERROR_SUCCESS) { // checking if the controller is disconnected by updating current state
			std::cout << "Controller has been disconnected. Reconnect the controller to resume.\n";
			controllerID = -1;
			return false;
		}
	}

	/* some math to map thumbstick values to range from -1 to 1 */
	// left thumbstick
	float normLX = fmaxf(-1, (float)currentState.Gamepad.sThumbLX / 32767);
	float normLY = fmaxf(-1, (float)currentState.Gamepad.sThumbLY / 32767);
	float leftStickX = (abs(normLX) < deadzoneX_LT ? 0 : (abs(normLX) - deadzoneX_LT) * (normLX / abs(normLX)));
	float leftStickY = (abs(normLY) < deadzoneY_LT ? 0 : (abs(normLY) - deadzoneY_LT) * (normLY / abs(normLY)));

	leftStickX *= 1 / (1 - deadzoneX_LT);
	leftStickY *= 1 / (1 - deadzoneY_LT);

	// right thumbstick
	float normRX = fmaxf(-1, (float)currentState.Gamepad.sThumbRX / 32767);
	float normRY = fmaxf(-1, (float)currentState.Gamepad.sThumbRY / 32767);
	float rightStickX = (abs(normRX) < deadzoneX_RT ? 0 : (abs(normRX) - deadzoneX_RT) * (normRX / abs(normRX)));
	float rightStickY = (abs(normRY) < deadzoneY_RT ? 0 : (abs(normRY) - deadzoneY_RT) * (normRY / abs(normRY)));

	rightStickX *= 1 / (1 - deadzoneX_RT);
	rightStickY *= 1 / (1 - deadzoneY_RT);

	// setting the struct to contain current thumbstick states
	thumbSticks.LTx = leftStickX;
	thumbSticks.LTy = leftStickY;
	thumbSticks.RTx = rightStickX;
	thumbSticks.RTy = rightStickY;


	/* now the controller state is updated so can start performing actions */
	UpdateScene();

	return true;
}

void XInputWrapper::Start(int refreshRate) {
	float sleepTime = 1000 / refreshRate;

	while (Refresh()) { // start the loop
		Sleep(sleepTime);
	}
}