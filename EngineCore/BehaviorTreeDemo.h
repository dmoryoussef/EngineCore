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

	void execute(float fDeltaTime)
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

	void execute(float fDeltaTime)
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

class MoveBehaviorNode : public LeafNode
{
private:
	Vector2 vTargetLocation;
	BaseNode* pEntity;

	string description()
	{
		if (m_nState == RUNNING)
			return "Moving to " + vTargetLocation.toString() + ".";

		if (m_nState == SUCCESS)
			return "Arrived at location.";

		return "Target location not reachable.";	//	failure
	}

public:
	MoveBehaviorNode(BaseNode* entity) :
		pEntity(entity) {};

	void setTarget(Vector2 target)
	{
		vTargetLocation = target;
	}

	void execute(float fDeltaTime)
	{
		//	get current location
		if (Transform2D* pTransform = pEntity->getChild<Transform2D>())
		{
			//	move to target action
			//		accellerate force?
			//		move directly?

			if (Accelerate* accel = pEntity->getChild<Accelerate>())
			{
				Vector2 pos = pTransform->getPosition();
				float fDistance = distance(vTargetLocation, pos);
				float speed = 0.0001 * fDeltaTime;
				if (fDistance > 1.0)
				{
					accel->setForce((vTargetLocation - pos).normalize() * speed);
				}
				else
				{
					speed = 0;
					accel->setForce((vTargetLocation - pos).normalize() * speed);

					m_nState = SUCCESS;
				}
			}
		}
	}
};

class BehaviorTreeDemo : public GameState
{
private:
	BehaviorNode* bt;
	Transform3D* pCameraTransform;

	BaseNode* pEntity;

public:
	BehaviorTreeDemo() :
		bt(NULL),
		pEntity(NULL),
		pCameraTransform(NULL) {};

	BehaviorNode* buildBehaviorTree()
	{
		/*BehaviorNode* nodeA = new SequenceNode();
		nodeA->addChild(new MouseBehaviorNode());
		nodeA->addChild(new KeyBehaviorNode());
		nodeA->addChild(new MouseBehaviorNode());

		BehaviorNode* nodeB = new SequenceNode();
		nodeB->addChild(new MouseBehaviorNode());
		nodeB->addChild(new KeyBehaviorNode());
		nodeB->addChild(new MouseBehaviorNode());*/

		BehaviorNode* node = new SequenceNode();
		/*node->addChild(nodeA);
		node->addChild(nodeB);*/

		MoveBehaviorNode* move = new MoveBehaviorNode(pEntity);
		move->setTarget({ 15, 15 });
		node->addChild(move);

		return node;
	}

	void start(BaseNode* pData, BaseNode* pSystems, BaseNode* pGUI)
	{
		int height = pGUI->cast<_UIComponent>()->getHeight();
		int width = pGUI->cast<_UIComponent>()->getWidth();
		CameraViewWindow* pCameraWindow = new CameraViewWindow(width, height, 0, 0);
		BaseNode* pCamera = pCameraWindow->getCamera();
		pCameraTransform = pCamera->getChild<Transform3D>();
		pData->add(pCamera);
		pGUI->addAtEnd(pCameraWindow);

		pEntity = pSystems->getChild<EntityFactory>()->createPlayer(0);
		pData->add(pEntity);
		
		bt = buildBehaviorTree();
	}

	void update(BaseNode* pData, float fDeltaTime)
	{
		bt->update(fDeltaTime);
	}

	void render(OutputBuffer* pEngineBuffer)
	{
		Render2D renderer(pEngineBuffer, pCameraTransform->getPosition());
		
		bt->render(&renderer, 25, 1);
	}
};