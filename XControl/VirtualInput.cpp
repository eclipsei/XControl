
#include "VirtualInput.h"

void VirtualInput::setMouseMoveSpeed(int speed) {
	mouseMoveSpeed = speed;
}

void VirtualInput::setScrollSpeed(int speed) {
	scrollSpeed = speed;
}

void VirtualInput::MouseLeftDown() {
	INPUT    Input;
	ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &Input, sizeof(INPUT));
}

void VirtualInput::MouseLeftUp() {
	INPUT    Input;
	ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &Input, sizeof(INPUT));
}

void VirtualInput::MouseRightDown() {
	INPUT    Input;
	ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	SendInput(1, &Input, sizeof(INPUT));
}

void VirtualInput::MouseRightUp() {
	INPUT    Input;
	ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	SendInput(1, &Input, sizeof(INPUT));
}

void VirtualInput::MouseX1Down() {
	INPUT    Input;
	ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_XDOWN;
	Input.mi.mouseData = XBUTTON1;
	SendInput(1, &Input, sizeof(INPUT));
}

void VirtualInput::MouseX1Up() {
	INPUT    Input;
	ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_XUP;
	Input.mi.mouseData = XBUTTON1;
	SendInput(1, &Input, sizeof(INPUT));
}

void VirtualInput::MouseX2Down() {
	INPUT    Input;
	ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_XDOWN;
	Input.mi.mouseData = XBUTTON2;
	SendInput(1, &Input, sizeof(INPUT));
}

void VirtualInput::MouseX2Up() {
	INPUT    Input;
	ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_XUP;
	Input.mi.mouseData = XBUTTON2;
	SendInput(1, &Input, sizeof(INPUT));
}

void VirtualInput::MoveMouse(float x, float y) {

	INPUT Input;
	ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dx = mouseMoveSpeed*x;
	Input.mi.dy = -mouseMoveSpeed*y; // minus because positive dy value goes downwards

	Input.mi.dwFlags = MOUSEEVENTF_MOVE;
	SendInput(1, &Input, sizeof(INPUT));
}

void VirtualInput::ScrollWheelDown() {
	INPUT    Input;
	ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
	SendInput(1, &Input, sizeof(INPUT));
}

void VirtualInput::ScrollWheelUp() {
	INPUT    Input;
	ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
	SendInput(1, &Input, sizeof(INPUT));
}

void VirtualInput::ScrollWheelMove(float y) {
	INPUT Input;
	ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_WHEEL;
	Input.mi.mouseData = scrollSpeed*y;

	SendInput(1, &Input, sizeof(INPUT));
}