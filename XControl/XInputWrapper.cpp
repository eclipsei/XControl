
#include "XInputWrapper.h"

/* Private functions */

void XInputWrapper::UpdateScene() {

	VirtualInput vi = VirtualInput();

	for (std::vector<MappedKey>::iterator it = mapConfiguration.begin(); it != mapConfiguration.end(); ++it) {
		int type = it->type;
		auto vk = it->virtualKey;
		auto button = it->controllerButton;

		switch (type) { // depending on type of input (keyboard or mouse)
		case MAP_TYPE_MOUSE: // handle mouse mappings

			switch (vk) { // depending on which mouse button

			case VK_LBUTTON:
				if (IsPressed(button, currentState)) { // if pressed in the current state
					if (!IsPressed(button, previousState)) { // if not pressed in the previous state
						vi.MouseLeftDown();
					}
				}
				else { // if not pressed in the current state
					if (IsPressed(button, previousState)) { // if pressed in the previous state
						vi.MouseLeftUp();
					}
				}
				break;

			case VK_RBUTTON:
				if (IsPressed(button, currentState)) { // if pressed in the current state
					if (!IsPressed(button, previousState)) { // if not pressed in the previous state
						vi.MouseRightDown();
					}
				}
				else { // if not pressed in the current state
					if (IsPressed(button, previousState)) { // if pressed in the previous state
						vi.MouseRightUp();
					}
				}
				break;

			case VK_MBUTTON:
				if (IsPressed(button, currentState)) { // if pressed in the current state
					if (!IsPressed(button, previousState)) { // if not pressed in the previous state
						vi.ScrollWheelDown();
					}
				}
				else { // if not pressed in the current state
					if (IsPressed(button, previousState)) { // if pressed in the previous state
						vi.ScrollWheelUp();
					}
				}
				break;

			case VK_XBUTTON1:
				if (IsPressed(button, currentState)) { // if pressed in the current state
					if (!IsPressed(button, previousState)) { // if not pressed in the previous state
						vi.MouseX1Down();
					}
				}
				else { // if not pressed in the current state
					if (IsPressed(button, previousState)) { // if pressed in the previous state
						vi.MouseX1Up();
					}
				}
				break;

			case VK_XBUTTON2:
				if (IsPressed(button, currentState)) { // if pressed in the current state
					if (!IsPressed(button, previousState)) { // if not pressed in the previous state
						vi.MouseX2Down();
					}
				}
				else { // if not pressed in the current state
					if (IsPressed(button, previousState)) { // if pressed in the previous state
						vi.MouseX2Up();
					}
				}
				break;

			default:
				std::cout << "Update scene unhandled case (MOUSE)!\n";
				break;
			}

			break;


		case MAP_TYPE_KEYBOARD: // handle keyboard mappings
			if (IsPressed(button, currentState)) { // if pressed in the current state
				if (!IsPressed(button, previousState)) { // if not pressed in the previous state
					vi.keyDown(vk);
				}
			}
			else { // if not pressed in the current state
				if (IsPressed(button, previousState)) { // if pressed in the previous state
					vi.keyUp(vk);
				}
			}
			break;

		default:
			std::cout << "Update scene unhandled case (KEYBOARD)!\n";
			break;
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

XINPUT_GAMEPAD* XInputWrapper::GetPreviousState() {
	return &previousState.Gamepad;
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

void XInputWrapper::InitConfiguration() {
	MappedKey mk;
	mk.type = MAP_TYPE_MOUSE;
	mk.controllerButton = XINPUT_GAMEPAD_A;
	mk.virtualKey = VK_LBUTTON;

	mapConfiguration.push_back(mk);

	mk.type = MAP_TYPE_MOUSE;
	mk.controllerButton = XINPUT_GAMEPAD_B;
	mk.virtualKey = VK_RBUTTON;

	mapConfiguration.push_back(mk);

	mk.type = MAP_TYPE_MOUSE;
	mk.controllerButton = XINPUT_GAMEPAD_Y;
	mk.virtualKey = VK_MBUTTON;

	mapConfiguration.push_back(mk);

	mk.type = MAP_TYPE_MOUSE;
	mk.controllerButton = XINPUT_GAMEPAD_LEFT_SHOULDER;
	mk.virtualKey = VK_XBUTTON1;

	mapConfiguration.push_back(mk);

	mk.type = MAP_TYPE_MOUSE;
	mk.controllerButton = XINPUT_GAMEPAD_RIGHT_SHOULDER;
	mk.virtualKey = VK_XBUTTON2;

	mapConfiguration.push_back(mk);

	mk.type = MAP_TYPE_KEYBOARD;
	mk.controllerButton = XINPUT_GAMEPAD_X;
	mk.virtualKey = VK_RETURN;

	mapConfiguration.push_back(mk);

}
