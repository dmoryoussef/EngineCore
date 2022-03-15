class _InputDevice
{
	//	keyboard input x 1
	//	mouse input x 1
	//	game pad input x 4
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

class ConsoleInputBuffer : public EventListener
{
private:
	HANDLE m_hInputHandle;
	INPUT_RECORD* m_pInputBuffer;
	int m_nTotalEvents;

public:


	ConsoleInputBuffer() :
		m_pInputBuffer(NULL),
		m_nTotalEvents(0)
	{
		m_hInputHandle = GetStdHandle(STD_INPUT_HANDLE);
	};

	INPUT_RECORD* getInput()
	{
		return m_pInputBuffer;
	}

	void getConsoleInput()
	{
		delete[] m_pInputBuffer;
		m_pInputBuffer = NULL;

		DWORD wTotalEvents = 0;
		DWORD wEventsRead = 0;

		GetNumberOfConsoleInputEvents(m_hInputHandle, &wTotalEvents);

		if (wTotalEvents != 0)
		{
			m_pInputBuffer = new INPUT_RECORD[wTotalEvents];
			ReadConsoleInput(m_hInputHandle, m_pInputBuffer, wTotalEvents, &wEventsRead);
			m_nTotalEvents = wTotalEvents;
		}
	}

	void convertEvents()
	{
		if (m_pInputBuffer)
			for (int nI = 0; nI < m_nTotalEvents; nI++)
			{
				switch (m_pInputBuffer[nI].EventType)
				{
					case KEY_EVENT:
					{
						char chKey = m_pInputBuffer[nI].Event.KeyEvent.uChar.AsciiChar;
						addEvent(new KeyboardEvent(chKey, m_pInputBuffer[nI].Event.KeyEvent.bKeyDown));
					}
					break;

					case MOUSE_EVENT:
					{
						MouseState State;

						int value = 0;
						if (m_pInputBuffer[nI].Event.MouseEvent.dwEventFlags == MOUSE_WHEELED)
						{
							value = m_pInputBuffer[nI].Event.MouseEvent.dwButtonState;
							if (value > 0)
								State.bWheeledUp = true;
							else
								State.bWheeledDown = true;
						}

						if (m_pInputBuffer[nI].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
							State.bLeftButtonDown = true;
						if (m_pInputBuffer[nI].Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
							State.bRightButtonDown = true;
						if (m_pInputBuffer[nI].Event.MouseEvent.dwControlKeyState == SHIFT_PRESSED)
							State.bShiftDown = true;
						if (m_pInputBuffer[nI].Event.MouseEvent.dwButtonState == FROM_LEFT_2ND_BUTTON_PRESSED)
							State.bWheelButtonDown = true;


						State.Position.Y = m_pInputBuffer[nI].Event.MouseEvent.dwMousePosition.Y;
						State.Position.X = m_pInputBuffer[nI].Event.MouseEvent.dwMousePosition.X;

						addEvent(new MouseEvent(State));
					}
					break;
				}
			}
	}


};