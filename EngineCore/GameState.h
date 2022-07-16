class GameState : public EventListener
{
public:
	GameState() {};

	virtual void start(BaseNode* pData, BaseNode* pSystems, BaseNode* pGUI) {}

	virtual void render(OutputBuffer *pEngineBuffer) {}

	virtual void update(BaseNode *pData, float fDeltaTime) {}
};

#include "TestState.h"

class StateManager : public EventListener
{
private:
	GameState* m_pCurrentState;

	vector<GameState> StateStack;

	void onEvent(_Event* pEvent)
	{
		//	change states
		//	other stuff
	}

public:
	StateManager() :
		m_pCurrentState(new TestState()) {};

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
