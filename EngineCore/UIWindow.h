class UIWindow :
	public _UIComponent
{
protected:
	bool m_bDraggable;
	bool m_bDragActive;
	bool m_bDisplayPosition;
	bool m_bDisplayTitle;


	Vector2 vCurrentMousePosition;
	Vector2 vPreviousMousePosition;

	Vector2 vStartDrag;

	string formatTitle(string strName)
	{
		string strFinal = "";
		strFinal.insert(0, 1, 181);
		strFinal.insert(strFinal.end(), 1, 198);

		strFinal.insert(1, strName);

		return strFinal;
	}

	string formatPosition()
	{
		string strPos = toString<int>(getMin().X) + "x" + toString<int>(getMin().Y);
		string strFinal = "";
		strFinal.insert(0, 1, 181);
		strFinal.insert(strFinal.end(), 1, 198);

		strFinal.insert(1, strPos);

		return strFinal;
	}

	void handleWindowMove(MouseEvent* pMouseEvent)
	{
		if (!m_bDragActive)
		{
			if (m_bMouseOver)	//	or window underneith will be grabbed too
				if (pMouseEvent->getState().Position.Y == getMin().Y
					&& pMouseEvent->getState().Position.X > getMin().X
					&& pMouseEvent->getState().Position.X < getMax().X
					&& pMouseEvent->getState().bLeftButtonDown)
				{
					vStartDrag = vCurrentMousePosition - Position;
					m_bDragActive = true;
				}
		}
		else
		{
			Position = vCurrentMousePosition - vStartDrag;
			if (!pMouseEvent->getState().bLeftButtonDown)
				m_bDragActive = false;
		}
	}

	void onEvent(_Event* pEvent)
	{
		_UIComponent::onEvent(pEvent);

		switch (pEvent->m_eType)
		{
			case CONSOLE_MOUSE_EVENT:
			{

				// convert to component
				//	drag: area, event listener
				//	close: area, event listener, 'x'
				//	resize window, etc
				MouseState mouseState = pEvent->get<MouseEvent>()->getState();
				vCurrentMousePosition = mouseState.Position;
				

				if (m_bDraggable)
					handleWindowMove(pEvent->get<MouseEvent>());

				if (m_bMouseOver && pEvent->get<MouseEvent>()->getState().bLeftButtonDown)
					moveToTop();


				break;
			}

		}
	}

	void constructBase(OutputBuffer *pBuffer)
	{
		int nForgroundColor = FG_DARKGREY;
		if (m_bMouseOver)
			nForgroundColor = FG_WHITE;

		int nFinalColor = m_nBGColor + nForgroundColor;

		drawBorder(pBuffer, nFinalColor);

		string strPosition = formatPosition();

		if (m_bDisplayPosition)
			pBuffer->set(strPosition, m_nWidth - (strPosition.size() + 1), 0, nFinalColor);

		if (m_bDisplayTitle)
			pBuffer->set(formatTitle(m_strText), 1, 0, nFinalColor);
	}

	

public:
	UIWindow(int nWidth, int nHeight, int nPosX, int nPosY) :
		m_bDragActive(false),
		m_bDisplayPosition(false),
		m_bDisplayTitle(false),
		m_bDraggable(true),
		_UIComponent(nWidth, nHeight, nPosX, nPosY)
	{
		setName("DEFAULT_WINDOW");
		setText("DEFAULT_WINDOW");
	};

	UIWindow(string strTitle, int nWidth, int nHeight, int nPosX, int nPosY) :
		m_bDragActive(false),
		m_bDisplayPosition(true),
		m_bDraggable(true),
		m_bDisplayTitle(true),
		_UIComponent(nWidth, nHeight, nPosX, nPosY)
	{
		setName(strTitle);
		setText(strTitle);
	};

	UIWindow() :
		m_bDragActive(false),
		m_bDisplayPosition(true),
		m_bDisplayTitle(true),
		m_bDraggable(false),
		_UIComponent(50, 50, 0, 0)
	{
		setName("DEFAULT_WINDOW");
		setText("DEFAULT_WINDOW");
	};


	void setDraggable(bool bDraggable)
	{
		m_bDraggable = bDraggable;
	}
	void setTitle(string strTitle)
	{
		m_strText = strTitle;
	}
	void setDisplayTitle(bool bDisplay)
	{
		m_bDisplayTitle = bDisplay;
	}
	void setDisplayPosition(bool bDisplay)
	{
		m_bDisplayPosition = bDisplay;
	}

	void moveToTop()
	{
		if (getParent())	// makes sure its not the base window, may get rid of this and draw directly to buffer
		{
			//	while not parent != null
			//	get parent
			//	 then move to top
			BaseNode* pParent = getParent();
			pop();
			pParent->addAtEnd(this);
			setParent(pParent);
		}
	}
	void updateMouseOver(Vector2 MousePosition)
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

	void resetMouseOver(_UIComponent* pComponent)
	{
		if (pComponent->isMouseOver())
			m_bMouseOver = false;

		for (_UIComponent* pCurrent = pComponent->getStart<_UIComponent>();		//	if any (window component) on top is mouse over - reset this 
			pCurrent != NULL;
			pCurrent = pCurrent->getNext<_UIComponent>())

			resetMouseOver(pCurrent);
	}

	void setMouseOver(bool bNewMouseOverState)
	{
		if (!m_bDragActive)												//	if not already dragging this window, update mouseover state
		{
			m_bMouseOver = bNewMouseOverState;

			//	if any (window component) on top is mouse over - reset this 

			// if top window - reset any other underneith that are mouseover

			if (m_bMouseOver == true)
			{
				// reset anything up and under
				//
			}
			/*	for (_AComponent *pCurrent = getPrevious<_AComponent>();
					pCurrent != NULL;
					pCurrent = pCurrent->getPrevious<_AComponent>())
						resetMouseOver(pCurrent);*/

			for (_UIComponent* pCurrent = getNext<_UIComponent>();
				pCurrent != NULL;
				pCurrent = pCurrent->getNext<_UIComponent>())
				resetMouseOver(pCurrent);


		}
	}

};

