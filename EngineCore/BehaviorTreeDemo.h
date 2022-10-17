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
		if (m_nState == IDLE)
			return "Idle.";

		if (m_nState == RUNNING)
			return "Waiting for left mouse button to be pressed.";
		
		if (m_nState == SUCCESS)
			return "Left mouse button was pressed.";

		return "Failure cant really happen here";
	}

	int execute(float fDeltaTime)
	{
		if (m_bMousePressed)
			return SUCCESS;
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

	int execute(float fDeltaTime)
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

class MoveBehaviorNode : public LeafNode, public EventListener
{
private:
	Vector2 vTargetLocation;
	BaseNode* pEntity;

	vector<Vector2> vPath;

	string description()
	{
		if (m_nState == RUNNING)
			return "Moving to " + vPath[0].toString() + ".";

		if (m_nState == SUCCESS)
			return "Arrived at location.";

		return "Target location not reachable.";	//	failure
	}

	void onMouseWorldEvent(MouseWorldEvent* pEvent)
	{
		if (pEvent->getState().bRightButtonDown)
			addPath(pEvent->getWorldPosition());
	}

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case MOUSEWORLD_EVENT: onMouseWorldEvent(pEvent->get<MouseWorldEvent>());
				break;
		}
	}

public:
	MoveBehaviorNode(BaseNode* entity) :
		pEntity(entity) 
	{
		registerListener(MOUSEWORLD_EVENT);
	};

	void setPath(vector<Vector2> path)
	{
		vPath = path;
	}

	void addPath(Vector2 node)
	{
		vPath.push_back(node);
	}

	void setTarget(Vector2 target)
	{
		vTargetLocation = target;
	}

	int execute(float fDeltaTime)
	{
		if (pEntity == NULL)
		{
			return FAILURE;
		}

		if (Transform2D* pTransform = pEntity->getChild<Transform2D>())
		{
			if (Accelerate* accel = pEntity->getChild<Accelerate>())
			{
				if (vPath.size() > 0)
				{
					Vector2 target = vPath[0];
					Vector2 pos = pTransform->getPosition();

					float fDistance = distance(target, pos);
					float speed = 0.00001 * fDeltaTime;
					Vector2 direction = (target - pos).normalize();
					if (fDistance > 1.0)
					{
						Vector2 force = direction * speed;
						pTransform->setRotation(direction);
						accel->setForce(force);
						return RUNNING;
					}
					else
					{
						vPath.erase(vPath.begin());
						if (vPath.size() == 0)
						{
							Vector2 force = direction * 0;
							pTransform->setRotation(direction);
							accel->setForce(force);
							return SUCCESS;
						}
						return RUNNING;
					}
				}
				else
					return SUCCESS;
			}
		}	

		//	no movement components (transform, accel)
		return FAILURE;
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

		node->addChild(new MouseBehaviorNode());
		
		MoveBehaviorNode* moveA = new MoveBehaviorNode(pEntity);
		moveA->addPath({ 15, 15 });
		moveA->addPath({ 25, 15 });
		node->addChild(moveA);


		MoveBehaviorNode* moveB = new MoveBehaviorNode(pEntity);
		moveB->addPath({ 15, 25 });
		moveB->addPath({ 15, 15 });
		node->addChild(moveB);
		node->setState(RUNNING);

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
		
		bt->render(&renderer, 25, 5);
	}
};