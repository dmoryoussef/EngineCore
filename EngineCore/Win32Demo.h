class Win32Demo : public GameState
{
private:
	BaseNode* player;

	void onGamePadEvent(GamePadEvent* pEvent)
	{
		//cout << "gamepad event";
	}


	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case GAMEPAD_EVENT: onGamePadEvent(pEvent->get<GamePadEvent>());
				break;
		}
	}

public:
	Win32Demo() 
	{
		registerListener(GAMEPAD_EVENT);
	}

	void start(BaseNode* pData, BaseNode* pSystems, BaseNode* pGUI)
	{
		cout << "Starting game state. \n";
		player = pSystems->getChild<EntityFactory>()->createPlayer(0);
		pData->add(player);
	}

	void update(BaseNode* pData, float fDeltaTime) 
	{

	}

	void render(OutputBuffer* pEngineBuffer)
	{
		//	Render2D renderer(pEngineBuffer);
		Transform2D *t = player->getChild<Transform2D>();
		float xPos = t->getPosition().X;
		float yPos = t->getPosition().Y;

		pEngineBuffer->DrawQuad(xPos, yPos, xPos + 100, yPos + 100, 255, 0, 0);
	}
};