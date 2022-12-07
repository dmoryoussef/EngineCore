class ConsoleInputHandler : public PlatformInputHandler
{
private:
	HANDLE m_hInputHandle;
	INPUT_RECORD* m_pInputBuffer;
	int m_nTotalEvents;

	bool bKeyPressed[256];

public:
	ConsoleInputHandler() :
		m_pInputBuffer(NULL),
		m_nTotalEvents(0)
	{
		m_hInputHandle = GetStdHandle(STD_INPUT_HANDLE);
		for (int i = 0; i < 256; i++)
		{
			bKeyPressed[i] = false;
		}
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
					case WINDOW_BUFFER_SIZE_EVENT:
					{
						m_pInputBuffer[nI].Event.WindowBufferSizeEvent;
					}break;

					case KEY_EVENT:
					{
						char chKey = m_pInputBuffer[nI].Event.KeyEvent.uChar.AsciiChar;
						
						bKeyPressed[chKey] = m_pInputBuffer[nI].Event.KeyEvent.bKeyDown;

						addEvent(new KeyboardEvent(chKey, m_pInputBuffer[nI].Event.KeyEvent.bKeyDown));
					}break;
					
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
						if (m_pInputBuffer[nI].Event.MouseEvent.dwButtonState == FROM_LEFT_2ND_BUTTON_PRESSED)
							State.bWheelButtonDown = true;

						if (m_pInputBuffer[nI].Event.MouseEvent.dwControlKeyState == SHIFT_PRESSED)
							State.bShiftDown = true;

						State.Position.Y = m_pInputBuffer[nI].Event.MouseEvent.dwMousePosition.Y;
						State.Position.X = m_pInputBuffer[nI].Event.MouseEvent.dwMousePosition.X;

						addEvent(new MouseEvent(State));
					}break;
				}
			}
	}

	void processInput()
	{
		getConsoleInput();
		convertEvents();
	}
};