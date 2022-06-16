//	class component child interface
//		close parent
//		scroll parent
//		drag parent
//		other function?

enum GUI_ALIGNMENT
{
	ALIGN_NONE,
	ALIGN_TOP,
	ALIGN_BOTTOM,
	ALIGN_LEFT,
	ALIGN_CENTERLEFT,
	ALIGN_RIGHT,
	ALIGN_CENTER,
	ALIGN_OUTSIDE_LEFT,
	ALIGN_OUTSIDE_RIGHT
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



class _UIComponent :
	public EventListener,
	public BaseNode,
	public ConsoleOutputBuffer
{
protected:
	Vector2 MaxSize;			//	any size greater than this - add scroll bar
	Vector2 Position;
	Vector2 Border;
	string m_strText;			// window title, button name, textfield, etc
	bool m_bMouseOver;

	int m_nState;
	bool m_bActive;
	bool m_bToggle;
	int m_nAlignment;

	int m_nBGColor;
	int m_nFGColor;

	virtual void removeComponent(_UIComponent* pComponent) {}
	virtual void constructComponent(BaseNode* pData) {}
	virtual void onStateChange() {};

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case CONSOLE_MOUSE_EVENT:
			{
				updateMouseOver(pEvent->get<MouseEvent>()->getState().Position);

				switch (m_nState)
				{
					case DEFAULT:
						if (m_bMouseOver)
							setState(MOUSE_OVER);
						break;

					case MOUSE_OVER:
						if (pEvent->get<MouseEvent>()->getState().bLeftButtonDown)
							setState(LEFT_PRESSED);
						if (!m_bMouseOver)
							setState(DEFAULT);
						break;

					case LEFT_PRESSED:		//	handle toggle between active/inactive
						if (!pEvent->get<MouseEvent>()->getState().bLeftButtonDown)
						{
							if (m_bMouseOver)
							{
								if (m_bActive == false)
								{
									setState(LEFT_RELEASED);
									m_bActive = true;

								}
								else
								{
									setState(DEFAULT);
									m_bActive = false;
								}
							}
							else							// if mouse is moved off, then button released - revert to previous state
							{
								if (m_bActive)
									setState(LEFT_RELEASED);
								else
									setState(DEFAULT);
							}
						}

						break;

					case LEFT_RELEASED:
						//	do something
						if (m_bToggle == false)
							if (m_bMouseOver)
								setState(MOUSE_OVER);
							else
								setState(DEFAULT);
						if (m_bMouseOver)
							if (pEvent->get<MouseEvent>()->getState().bLeftButtonDown)
								setState(LEFT_PRESSED);
						break;

					case RIGHT_RELEASED:
						//	do something
						if (m_bMouseOver)
							setState(MOUSE_OVER);
						else
							setState(DEFAULT);
						break;

				}
				break;
			}
		}
	}

	void drawBorder(int nFinalColor)
	{
		for (int nY = 0; nY < m_nHeight; nY++)
			for (int nX = 0; nX < m_nWidth; nX++)
			{
				if (nY == 0 || nY == m_nHeight - 1)					//	top or bottom sides
					set(205, nX, nY, nFinalColor);
				if (nX == 0 || nX == m_nWidth - 1)					//	left or right sides
					set(186, nX, nY, nFinalColor);

				if (nY == 0 && nX == 0)								//	top left corner
					set(201, nX, nY, nFinalColor);
				if (nY == 0 && nX == m_nWidth - 1)					//	top right corner
					set(187, nX, nY, nFinalColor);
				if (nY == m_nHeight - 1 && nX == m_nWidth - 1)		//	bottom right corner
					set(188, nX, nY, nFinalColor);
				if (nY == m_nHeight - 1 && nX == 0)					//	bottom left corner
					set(200, nX, nY, nFinalColor);
			}
	}

	void drawThinBorder(int nFinalColor)
	{
		for (int nY = 0; nY < m_nHeight; nY++)
			for (int nX = 0; nX < m_nWidth; nX++)
			{
				if (nY == 0 || nY == m_nHeight - 1)					//	top or bottom sides
					set(196, nX, nY, nFinalColor);
				if (nX == 0 || nX == m_nWidth - 1)					//	left or right sides
					set(179, nX, nY, nFinalColor);

				if (nY == 0 && nX == 0)								//	top left corner
					set(218, nX, nY, nFinalColor);
				if (nY == 0 && nX == m_nWidth - 1)					//	top right corner
					set(191, nX, nY, nFinalColor);
				if (nY == m_nHeight - 1 && nX == m_nWidth - 1)			//	bottom right corner
					set(217, nX, nY, nFinalColor);
				if (nY == m_nHeight - 1 && nX == 0)					//	bottom left corner
					set(192, nX, nY, nFinalColor);
			}
	}

	virtual void constructBase() {}

public:
	_UIComponent(int nWidth, int nHeight, int nPosX, int nPosY) :
		m_nState(DEFAULT),
		Position(nPosX, nPosY),
		MaxSize(nWidth, nHeight),
		Border(1, 0),
		m_bMouseOver(false),
		m_bActive(false),
		m_nAlignment(ALIGN_NONE),
		m_bToggle(true),
		m_nBGColor(BG_BLACK),
		m_nFGColor(FG_WHITE),
		ConsoleOutputBuffer(nWidth, nHeight)
	{
		setName("COMPONENT");
		registerListener(this, CONSOLE_MOUSE_EVENT);
		registerListener(this, CONSOLE_KEYBOARD_EVENT);

		registerListener(this, GUI_EVENT);
	};

	_UIComponent(int nWidth, int nHeight) :
		m_nState(DEFAULT),
		m_nAlignment(ALIGN_LEFT),
		Position(0, 0),
		MaxSize(0, 0),
		Border(0, 0),
		m_bMouseOver(false),
		m_bActive(false),
		m_bToggle(true),
		ConsoleOutputBuffer(nWidth, nHeight)
	{
		registerListener(this, CONSOLE_MOUSE_EVENT);
		registerListener(this, GUI_EVENT);
		registerListener(this, CONSOLE_KEYBOARD_EVENT);
	};

	_UIComponent(string strText) :
		m_nState(DEFAULT),
		m_nAlignment(ALIGN_LEFT),
		Position(0, 0),
		MaxSize(0, 0),
		Border(0, 0),
		m_bMouseOver(false),
		m_bActive(false),
		m_bToggle(true),
		ConsoleOutputBuffer(0, 0)
	{
		setName(strText);
		registerListener(this, CONSOLE_MOUSE_EVENT);
		registerListener(this, CONSOLE_KEYBOARD_EVENT);
		registerListener(this, GUI_EVENT);

		//	set position based on other
		//	components in list
	};

	_UIComponent() :
		m_nState(DEFAULT),
		m_nAlignment(ALIGN_NONE),
		Position(0, 0),
		MaxSize(0, 0),
		Border(0, 0),
		m_bMouseOver(false),
		m_bActive(false),
		m_bToggle(true),
		ConsoleOutputBuffer(0, 0)
	{
		setName("COMPONENT");
		registerListener(this, CONSOLE_MOUSE_EVENT);
		registerListener(this, CONSOLE_KEYBOARD_EVENT);
		registerListener(this, GUI_EVENT);

		//	set position based on other
		//	components in list
	};

	~_UIComponent()
	{
		unregisterAll();
	}

	int DefaultColor()
	{
		return m_nBGColor + m_nFGColor;
	}

	int getAlignment()
	{
		return m_nAlignment;
	}
	int getState()
	{
		return m_nState;
	}

	void setAlignment(int nAlignment)
	{
		m_nAlignment = nAlignment;
		int nAlignY = 1; //	adjust for border
		int nAlignX = 1;
		if (m_pParent)
		{
			while (m_pParent->isIterating())
			{
				//	if current's alignment == this alignment
				_UIComponent* pCurrent = m_pParent->getCurrent<_UIComponent>();
				if (pCurrent != this)
					if (pCurrent->getAlignment() == nAlignment)
					{
						nAlignY = nAlignY + pCurrent->m_nHeight;
					}
			}

			switch (nAlignment)
			{
				case ALIGN_TOP:
					break;
				case ALIGN_BOTTOM:
					/*int nX = 1;
					int nY = getParent<_AComponent>()->getSize().Y;
					Position = Vector2(nY - Size.Y, nX);*/
					break;
				case ALIGN_LEFT:
				{
					//	check other previous siblings for align left?
					int nX = 1;
					int nY = nAlignY;
					Position = Vector2(nX, nY);
				} break;
				case ALIGN_CENTERLEFT:
				{
					int nX = 1;
					int nY = nAlignY;
					nY = getParent<_UIComponent>()->m_nHeight;
					Position = Vector2(nX / 2 - m_nWidth, nY);
				} break;
				case ALIGN_RIGHT:
				{
					int nX = getParent<_UIComponent>()->m_nWidth;
					int nY = nAlignY;
					Position = Vector2(nX - m_nWidth - 1, nY);
				} break;
				case ALIGN_CENTER:
				{

				} break;
				case ALIGN_OUTSIDE_LEFT:
				{
					int nPosY = 0;
					int nPosX = 0;
					nPosX = nPosX - m_nWidth + 1;
					Vector2 NewPos(nPosX, nPosY);
					Position = NewPos;
				} break;
				case ALIGN_OUTSIDE_RIGHT:
					break;
			}
		}
	}
	void setPosition(int nX, int nY)
	{
		Position.X = nX;
		Position.Y = nY;
	}
	void setState(int nState)
	{
		m_nState = nState;
		if (m_nState == DEFAULT)
			m_bActive = false;

		// specialized event if state changed to something important
		onStateChange();
	}

	void render(BaseNode* pData, OutputBuffer* pFrame)
	{
		clear(m_nBGColor);

		// specific to each component (game data to be diplayed)
		constructComponent(pData);

		//	layer base on top (title, boarder, etc)
		//	only called here
		constructBase();

		//	set final look to main frame
		pFrame->set(this, getMin().X, getMin().Y);

		// Now do the same for each child component
		for (_UIComponent* pChild = getStart<_UIComponent>(); pChild != NULL; pChild = pChild->getNext<_UIComponent>())
		{
			pChild->render(pData, pFrame);
		}
	}
	void update(int nDeltaTime)
	{
		while (isIterating())
			getCurrent()->update(nDeltaTime);
	}

	template <typename T> string toString(T data)
	{
		stringstream str;
		str << data;
		return str.str();
	}

	template <typename T> T* getComponent()
	{
		return getChild<T>();
	}

	string getText()
	{
		return m_strText;
	}

	string toString()
	{
		return m_strName;
	}

	Vector2 getPosition()
	{
		return Position;
	}
	Vector2 getConsoleWindowPosition()
	{
		Vector2 RelPosition = Position;
		for (_UIComponent* pParent = getParent<_UIComponent>(); pParent != NULL; pParent = pParent->getParent<_UIComponent>())
		{
			Vector2 ParentPos = pParent->getPosition();
			RelPosition = RelPosition + ParentPos;
		}
		return RelPosition;
	}
	Vector2 getMin()
	{
		Vector2 Pos = getConsoleWindowPosition();
		return Pos;
	}
	Vector2 getMax()
	{
		Vector2 Position = getConsoleWindowPosition();
		Vector2 FinalMax = Position + Size;
		return  FinalMax;
	}

	void setToggle(bool bToggle)
	{
		m_bToggle = bToggle;
	}

	bool isMouseOver()
	{
		return m_bMouseOver;
	}

	virtual void updateMouseOver(Vector2 MousePosition)
	{
		if (getParent())									// MOVED THIS TO COMPONENT SPECIFIC 
			if (getParent<_UIComponent>()->isMouseOver())	//	only handle if the parent component is already mouseover
			{
				if (MousePosition.X >= getMin().X &&
					MousePosition.Y >= getMin().Y &&
					MousePosition.X < getMax().X &&
					MousePosition.Y < getMax().Y)
				{
					setMouseOver(true);
				}
				else
					setMouseOver(false);
			}
			else
			{
				setMouseOver(false);						//	Parent is not MouseOver so child should not be - needs to be explicitly set or there are bugs
			}
	}

	virtual void setText(string strText)
	{
		m_strText = strText;
	}
	virtual void setMouseOver(bool bMouseOver)
	{
		m_bMouseOver = bMouseOver;
	}
	virtual void addComponent(_UIComponent* pComponent)
	{
		addChild(pComponent);
		pComponent->setAlignment(pComponent->getAlignment());
	}
};

