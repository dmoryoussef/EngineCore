class TestState : public GameState
{
	//	this test state uses the prebuilt engine and entity components
	//	and adds some custom code inside its own class 
	//	to implement user defined behavior beyond the scope of the existing engine
	//	in this case: entity ships explode after their health drops to zero


private:
	BaseNode* Player1;
	BaseNode* Player2;

	BaseNode* entities;

public:
	TestState() :
		Player1(NULL),
		Player2(NULL) {};

	class Lifespan : public _EntityComponent
	{
	private:
		float time;
	public:
		Lifespan() :
			time(800), // in milliseconds
			_EntityComponent("LIFESPAN") {};

		void update(float fDeltaTime)
		{
			if (time > 0)
				time = time - fDeltaTime;
			else
			{
				addEvent(new DeleteBaseNodeEvent(m_pParent));
			}

		}
	};

	void explosion(Vector2 pos)
	{
		for (int i = 0; i < 10; i++)
		{
			Vector2 direction = random(0, 360) * (PI / 180);
			float vel = float(random(50, 60)) / 500;// fine tune particle velocity
			direction = direction * vel; 

			BaseNode* pTemp = new BaseNode();
			pTemp->add(new Transform2D(pos, {0, 0}, { 0.1, 0.1 }));
			pTemp->addChild(new Physics(direction));
			pTemp->addChild(new Render(1));
			pTemp->addChild(new Lifespan());
			entities->add(pTemp);
		}
	}

	void start(BaseNode* pData, BaseNode *pSystems, BaseNode *pGUI)
	{
		entities = pData;

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
				explosion(Player1->getChild<Transform2D>()->getPosition());
				Player1 = NULL;
			}

		if (Player2)
			if (Player2->getChild<Health>()->getHealth() <= 0)
			{
				addEvent(new DeleteBaseNodeEvent(Player2));
				explosion(Player2->getChild<Transform2D>()->getPosition());
				Player2 = NULL;
			}
	}
};