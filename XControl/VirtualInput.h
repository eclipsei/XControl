
#ifndef VIRTUAL_INPUT_
#define VIRTUAL_INPUT_

#include <Windows.h>

class VirtualInput {

private:

	int mouseMoveSpeed = 20;
	int scrollSpeed = 100;

public:

	void setMouseMoveSpeed(int);
	void setScrollSpeed(int);

	void MouseLeftDown(); // simulate left mouse button being pressed down
	void MouseLeftUp(); // simulate left mouse button being released
	void MouseRightDown(); // right mouse pressed
	void MouseRightUp(); // right mouse released
	void MouseX1Down(); // x1button down
	void MouseX1Up(); // x1button up
	void MouseX2Down(); // x2button down
	void MouseX2Up(); // x2button up
	void MoveMouse(float, float); // move mouse

	void ScrollWheelDown(); // scrollwheel pressed
	void ScrollWheelUp(); // scrollwheel released
	void ScrollWheelMove(float); // use scrollwheel, negative values move downwards

	void keyDown(WORD); // specified key down (see https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx for keycodes)
	void keyUp(WORD); // specified key up
};

#endif