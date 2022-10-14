class MouseBehaviorNode : public LeafNode, EventListener
{
private:
	bool m_bMousePressed;

	void onMouseEvent(MouseEvent* pEvent)
	{
		m_bMousePressed = pEvent->getState().bLeftButtonDown;
	}
	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case CONSOLE_MOUSE_EVENT: onMouseEvent(pEvent->get<MouseEvent>());
				break;
		}
	}

public:
	MouseBehaviorNode() : 
		LeafNode("MouseBehaviorLeaf")
	{
		registerListener(CONSOLE_MOUSE_EVENT);
	}

	string description()
	{
		if (m_nState == RUNNING)
			return "Waiting for left mouse button to be pressed.";
		
		if (m_nState == SUCCESS)
			return "Left mouse button was pressed.";

		return "Failure cant really happen here";
	}

	void execute()
	{
		if (m_bMousePressed)
			m_nState = SUCCESS;
	}
};

class KeyBehaviorNode : public LeafNode, EventListener
{
private:
	bool m_bKeyPressed;

	void onKeyboardEvent(KeyboardEvent* pEvent)
	{
		m_bKeyPressed = pEvent->isKeyDown();
	}
	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case KEYBOARD_EVENT: onKeyboardEvent(pEvent->get<KeyboardEvent>());
				break;
		}
	}

public:
	KeyBehaviorNode() :
		LeafNode("KeyBehaviorLeaf")
	{
		registerListener(KEYBOARD_EVENT);
	}

	void execute()
	{
		if (m_bKeyPressed)
			m_nState = SUCCESS;
	}

	string description()
	{
		if (m_nState == RUNNING)
			return "Waiting for keyboard button to be pressed.";

		if (m_nState == SUCCESS)
			return "Keyboard button was pressed.";

		return "Failure cant really happen here";
	}
};

class BehaviorTreeDemo : public GameState
{
private:
	BehaviorTree bt;

public:
	BehaviorTreeDemo() {};

	void start(BaseNode* pData, BaseNode* pSystems, BaseNode* pGUI)
	{
		SequenceNode* sq1 = new SequenceNode("SQ1");
		sq1->addChild(new MouseBehaviorNode());
		sq1->addChild(new KeyBehaviorNode());

		SequenceNode* sq2 = new SequenceNode("SQ2");
		sq2->addChild(new KeyBehaviorNode());
		sq2->addChild(new MouseBehaviorNode());

		SequenceNode* sq3 = new SequenceNode("SQ3");
		sq3->addChild(sq1);
		sq3->addChild(sq2);

		bt.root = sq3; 
			
	}

	void update(BaseNode* pData, float fDeltaTime)
	{
		bt.update();
	}

	void render(OutputBuffer* pEngineBuffer)
	{
		Render2D render(pEngineBuffer);
		
		bt.root->render(&render, 5, 1);
	}
};