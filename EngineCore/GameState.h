class GameState : public EventListener
{
protected:
	
public:
	GameState()
	{};

	virtual void start(BaseNode* pData, BaseNode* pSystems, BaseNode* pGUI) {}

	virtual void render(OutputBuffer *pEngineBuffer) {}

	virtual void update(BaseNode *pData, float fDeltaTime) {}
};


class StateManager : public EventListener
{
private:
	GameState* m_pCurrentState;

	vector<GameState> StateStack;
	int m_nBufferWidth;
	int m_nBufferHeight;
	void onEvent(_Event* pEvent)
	{
		//	change states
		//	other stuff
	}

public:
	StateManager() :
		m_nBufferHeight(0),
		m_nBufferWidth(0),
		m_pCurrentState(NULL) {};

	void setState(GameState* pState)
	{
		m_pCurrentState = pState;
	}

	void setEngineBufferSize(int width, int height)
	{
		m_nBufferHeight = height;
		m_nBufferWidth = width;
	}

	void start(BaseNode* pData, BaseNode* pSystems, BaseNode* pGUI)
	{
		if (m_pCurrentState)
			m_pCurrentState->start(pData, pSystems, pGUI);

	}

	void render(OutputBuffer* pEngineBuffer)
	{
		if (m_pCurrentState)
			m_pCurrentState->render(pEngineBuffer);
	}

	void update(BaseNode *pData, float fDeltaTime)
	{
		if (m_pCurrentState)
			m_pCurrentState->update(pData, fDeltaTime);
	}
};
