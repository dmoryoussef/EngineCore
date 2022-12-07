class UIButton :
	public _UIComponent
{
protected:
	void constructBase(OutputBuffer *pBuffer)
	{
		int nCenterX = (m_nWidth / 2) - (m_strText.size() / 2);
		int nCenterY = m_nHeight / 2;

		switch (m_nState)
		{
			case DEFAULT:
				//setForegroundColor(FOREGROUND_DARKGREY);
				//	setBackgroundColor(BACKGROUND_BLACK);
				pBuffer->set(m_strText, nCenterY, nCenterX, m_nBGColor + FG_DARKGREY);
				break;
			case MOUSE_OVER:
				//	setForegroundColor(FOREGROUND_BLACK);
				//	setBackgroundColor(BACKGROUND_WHITE);
				pBuffer->set(m_strText, nCenterY, nCenterX, BG_LIGHTGREY + FG_BLACK);
				break;
			case LEFT_PRESSED:
				//	setForegroundColor(FOREGROUND_WHITE);
				//	setBackgroundColor(BACKGROUND_BLACK);
				pBuffer->set(m_strText, nCenterY, nCenterX, m_nBGColor + FG_WHITE);
				break;
			case LEFT_RELEASED:
				//	setForegroundColor(FOREGROUND_BLACK);
				//	setBackgroundColor(BACKGROUND_WHITE);
				pBuffer->set(m_strText, nCenterY, nCenterX, BG_WHITE + FG_BLACK);
				break;
		}
	}

	void onEvent(_Event* pEvent)
	{
		_UIComponent::onEvent(pEvent);
	}

	void onStateChange()
	{
		addEvent(new GuiEvent(this, m_nState));
	}

public:
	UIButton(string strText, int nX, int nY) :
		_UIComponent(1, strText.size(), nX, nY)
	{
		setText(strText);

	};

	UIButton(int nWidth, int nHeight, int nPosX, int nPosY) :
		_UIComponent(nWidth, nHeight, nPosX, nPosY)
	{

	};

	UIButton(string strText, bool bToggle) :
		_UIComponent(strText.size(), 1, 0, 0)
	{
		m_bToggle = bToggle;
		setText(strText);

	};

	UIButton(string strText) :
		_UIComponent(strText.size(), 1, 0, 0)
	{
		setText(strText);
		EdgeBuffer = { 0, 0 };
		setAlignment(ALIGN_LEFT);
	};

	~UIButton()
	{
		unregisterAll();
	}

};