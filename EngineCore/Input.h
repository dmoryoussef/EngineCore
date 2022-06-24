class _InputDevice
{
	int nIndex;
	//	keyboard input x 1
	//	mouse input x 1
	//	game pad input x 4
};
enum GUI_STATE
{
	DEFAULT,
	MOUSE_OVER,
	LEFT_PRESSED,
	LEFT_RELEASED,
	RIGHT_PRESSED,
	RIGHT_RELEASED,
	LEFT_ACTIVE
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

	float LeftTrigger;
	float RightTrigger;

	float LeftStickX;
	float LeftStickY;

	float RightStickX;
	float RightStickY;

	float clearDeadzone(float val)
	{
		if (val > 0)
			if (val < .2)
				val = 0;
		if (val < 0)
			if (val > -0.2)
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

	int updateState(bool bMouseOver, bool bCurrentlyActive, int nCurrentState)
	{
		switch (nCurrentState)
		{
			case DEFAULT:
			{
				if (bMouseOver)
					return (MOUSE_OVER);
				break;
			}

			case MOUSE_OVER:
			{
				if (bLeftButtonDown)
					return (LEFT_PRESSED);
			
				if (!bMouseOver)
					return (DEFAULT);
				break;
			}

			case LEFT_PRESSED:
			{
				if (!bLeftButtonDown)
				{
					if (bMouseOver)
					{	//	handle toggle between active/inactive

						if (!bCurrentlyActive)
							return(LEFT_RELEASED);
						
						else
							return(MOUSE_OVER);
					}
					else							
					{
						// if mouse is moved off, then button released - revert to previous state
						if (bCurrentlyActive)
							return(LEFT_RELEASED);
						else
							return(DEFAULT);
					}
				}
				break;
			}

			case LEFT_RELEASED:
			{
				if (bMouseOver)
					if (bLeftButtonDown)
						return (LEFT_PRESSED);

				/*if (!bMouseOver)
					return (DEFAULT);*/
				
				break;
			}
		}

		return nCurrentState;
	}
};

class KeyboardState :
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

