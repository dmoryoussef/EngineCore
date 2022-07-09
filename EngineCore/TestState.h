class TestState : public GameState
{
private:
	BaseNode* Player1;
	BaseNode* Player2;

public:
	TestState() :
		Player1(NULL),
		Player2(NULL) {};

	void start(BaseNode* pData, BaseNode *pSystems, BaseNode *pGUI)
	{
		CameraViewWindow* pCameraWindow = new CameraViewWindow(100, 100, 0, 0);
		pData->add(pCameraWindow->getCamera());
		pGUI->addAtEnd(pCameraWindow);
		pGUI->addAtEnd(new ListExplorerWindow(pData));
		

		Player1 = pSystems->getChild<EntityFactory>()->createPlayer(0);
		Player2 = pSystems->getChild<EntityFactory>()->createPlayer(1);

		pData->add(Player1);
		pData->add(Player2);
	}

	void render(OutputBuffer* pEngineBuffer)
	{

	}

	void update(BaseNode *pData, float fDeltaTime)
	{
		if (Player1)
			if (Player1->getChild<Health>()->getHealth() <= 0)
			{
				addEvent(new DeleteBaseNodeEvent(Player1));
				//	add explosion effect?
				Player1 = NULL;
			}

		if (Player2)
			if (Player2->getChild<Health>()->getHealth() <= 0)
			{
				addEvent(new DeleteBaseNodeEvent(Player2));
				//	add explosion effect?
				Player2 = NULL;
			}
	}
};