class UIState : public _EntityComponent
{
private:
	int m_nState;

public:
	UIState() :
		m_nState(0),
		_EntityComponent("UI_STATE") 
	{
		registerListener(BASENODE_EVENT);
	};

	int getState() { return m_nState; }
};
