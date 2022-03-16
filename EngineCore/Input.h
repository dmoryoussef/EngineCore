class _InputDevice
{
	//	keyboard input x 1
	//	mouse input x 1
	//	game pad input x 4
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

class Gamepad :
	public _InputDevice
{
	bool bUp;
	bool bDown;
	bool bLeft;
	bool bRight;
	bool bStart;
	bool bBack;
	bool bLeftShoulder;
	bool bRightShoulder;
	bool bAButton;
	bool bBButton;
	bool bXButton;
	bool bYButton;

	int bLeftStickX;
	int bLeftStickY;

	int bRightStickX;
	int bRightStickY;

	bool bConnected;
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

