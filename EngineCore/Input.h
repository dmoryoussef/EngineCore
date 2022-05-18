class _InputDevice
{
	//	keyboard input x 1
	//	mouse input x 1
	//	game pad input x 4
};

struct GamePad
{
	int nIndex;
	bool Up;
	bool Down;
	bool Left;
	bool Right;
	bool Start;
	bool Back;
	bool LeftShoulder;
	bool RightShoulder;
	bool AButton;
	bool BButton;
	bool XButton;
	bool YButton;

	float LeftStickX;
	float LeftStickY;

	float RightStickX;
	float RightStickY;

	float clearDeadzone(float val)
	{
		if (val > 0)
			if (val < .4)
				val = 0;
		if (val < 0)
			if (val > -0.4)
				val = 0;

		return val;
	}
};

struct MouseState
{
	int nX;
	int nY;
	Vector2 Position;
	bool bLeftButtonDown = false;
	bool bRightButtonDown = false;
	bool bWheeledUp = false;
	bool bWheeledDown = false;
	bool bWheelButtonDown = false;
	bool bShiftDown = false;
};

class Keyboard :
	public _InputDevice
{
	//	keys[256]
	//	up/down
};


class Mouse :
	public _InputDevice
{
	int nX;
	int nY;

	// button 1
	// button 2
	// button 3
	// button 4
	// button 5

	// wheel up
	// wheel down
};

_InputDevice InputDevices[6];

