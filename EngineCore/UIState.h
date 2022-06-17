class UIState : public _EntityComponent
{
private:
	int m_nState;
	
public:
	UIState() :
		m_nState(0),
		_EntityComponent("UI_STATE") 
	{

	};

	int getState() { return m_nState; }

	void setState(int nState)
	{
		m_nState = nState;
	}
};
