class UIState : public _EntityComponent
{
private:
	int m_nCurrentState;
	bool m_bActive;

public:
	UIState() :
		m_nCurrentState(0),
		m_bActive(false),
		_EntityComponent("UI_STATE") {};

	int getState() { return m_nCurrentState; }

	void setState(int nNewState)
	{
		if (m_nCurrentState != nNewState)
		{
			if (m_bActive)
			{
				if (m_nCurrentState == LEFT_PRESSED)
				{
					if (nNewState == MOUSE_OVER)
						m_bActive = false;
				}
			}
			else
			{
				if (m_nCurrentState == LEFT_PRESSED)
				{
					if (nNewState == LEFT_RELEASED)
						m_bActive = true;
				}
			}
			
			m_nCurrentState = nNewState;
			addEvent(new GuiEvent(this));
		}
	}

	bool isActive() { return m_bActive; }

	void setActive(bool bActive)
	{
		m_bActive = bActive;
	}
};
