enum EVENT_TYPE
{
	CONSOLE_KEYBOARD_EVENT,
	CONSOLE_MOUSE_EVENT,
	GAMEPAD_EVENT,
	GUI_EVENT,
	NEW_BASENODE_EVENT,
	TRANSFORMUPDATE_EVENT,
	MOUSEWORLD_EVENT,
	SELECTIONSQUARE_EVENT,
	BASENODE_EVENT
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

class KeyboardEvent :
	public _Event
{
private:
	char m_chKey;
	bool m_bKeyDown;
public:
	KeyboardEvent(char chKey, bool bKeyDown) :
		m_chKey(chKey),
		m_bKeyDown(bKeyDown),
		_Event(CONSOLE_KEYBOARD_EVENT) {};

	bool isKeyDown()
	{
		return m_bKeyDown;
	}
	char getKey() { return m_chKey; }
};

class MouseEvent :
	public _Event
{
private:
	MouseState State;

public:
	MouseEvent(MouseState state) :
		State(state),
		_Event(CONSOLE_MOUSE_EVENT)
	{}

	MouseState getState()
	{
		return State;
	}
};
