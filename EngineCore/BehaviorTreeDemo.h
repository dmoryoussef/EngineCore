class MouseBehaviorNode : public LeafNode, EventListener
{
private:
	bool m_bMousePressed;

	void onMouseEvent(MouseWorldEvent* pEvent)
	{
		if (m_nState == RUNNING)
		{
			m_bMousePressed = pEvent->getState().bRightButtonDown;
			if (m_bMousePressed)
			{
				Vector2 position = pEvent->getWorldPosition();
				Blackboard->vPath.push_back(position);
			}
		}
	}
	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case MOUSEWORLD_EVENT: onMouseEvent(pEvent->get<MouseWorldEvent>());
				break;
		}
	}

public:
	MouseBehaviorNode() : 
		LeafNode("MouseBehaviorLeaf")
	{
		registerListener(MOUSEWORLD_EVENT);
	}

	void reset()
	{
		m_bMousePressed = false;
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

		
		return RUNNING;
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
	BaseNode* pEntity;
	bool bMouseUp;
	bool bMouseDown;

	string description()
	{
		switch (m_nState)
		{
			case RUNNING : 
				if (Blackboard->vPath.size() > 0)
					return "Moving to " + Blackboard->vPath[0].toString() + ".";
				else
					return "No path.";
				break;
			case SUCCESS : 
				return "Arrived at location.";
				break;
			case IDLE : 
				return "Waiting to move";
				break;
			case FAILURE : 
				return "Target location not reachable.";
				break;
		}
	}

	void onMouseWorldEvent(MouseWorldEvent* pEvent)
	{
		if (m_nState == RUNNING)
		{
			if (pEvent->getState().bRightButtonDown)
				bMouseDown = true;
			else
				if (bMouseDown)
				{
					bMouseUp = true;
					bMouseDown = false;
				}

			if (bMouseUp)
			{
				addToPath(pEvent->getWorldPosition());
				bMouseUp = false;
			}

		}
	}

	void onKeyboardEvent(KeyboardEvent* pEvent)
	{
		if (m_nState == RUNNING)
			if (pEvent->getKey() == 27)
				m_nState = FAILURE;
	}

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case MOUSEWORLD_EVENT: onMouseWorldEvent(pEvent->get<MouseWorldEvent>());
				break;
			case KEYBOARD_EVENT: onKeyboardEvent(pEvent->get<KeyboardEvent>());
				break;
		}
	}

public:
	MoveBehaviorNode(BaseNode* entity) :
		bMouseUp(false),
		bMouseDown(false),
		pEntity(entity) 
	{
		registerListener(MOUSEWORLD_EVENT);
		registerListener(KEYBOARD_EVENT);
	};

	void reset()
	{
		bMouseDown = false;
		bMouseUp = false;
		Blackboard->vPath.clear();
	}

	void setPath(vector<Vector2> path)
	{
		Blackboard->vPath = path;
	}

	void addToPath(Vector2 node)
	{
		Blackboard->vPath.push_back(node);
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
				if (Blackboard->vPath.size() > 0)
				{
					Vector2 target = Blackboard->vPath[0];
					Vector2 pos = pTransform->getPosition();

					float fDistance = distance(target, pos);
					float speed = 0.0001 * fDeltaTime;
					Vector2 direction = (target - pos).normalize();
					if (fDistance > 1.0)
					{
						Vector2 force = direction * speed;
						pTransform->setRotation({-direction.X, direction.Y});
						accel->setForce(force);
						return RUNNING;
					}
					else
					{
						Blackboard->vPath.erase(Blackboard->vPath.begin());
						if (Blackboard->vPath.size() == 0)
						{
							Vector2 force = direction * 0;
							pTransform->setRotation({ -direction.X, direction.Y });
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

	void renderNodeData(Render2D *renderer)
	{
		if (m_nState == RUNNING)
		{
			if (Blackboard->vPath.size() > 0)
			{
				for (Vector2 v : Blackboard->vPath)
				{
					renderer->DrawCircle(v.X, v.Y, 0.2, { PIXEL_SOLID, FG_LIGHTBLUE });
				}
				renderer->DrawCircle(Blackboard->vPath[0].X, Blackboard->vPath[0].Y, 0.5, {PIXEL_SOLID, FG_LIGHTGREEN});
			}
		}
	}
};

class RepeatDecorator : public DecoratorNode
{
private:
	bool m_bRepeat;

public:
	RepeatDecorator(string n = "Decorator", BehaviorTreeBlackboard* blackboard = NULL) :
		m_bRepeat(true),
		DecoratorNode(n, blackboard) {};

	virtual bool repeat()
	{
		return m_bRepeat;
	}

	string description()
	{
		switch (m_nState)
		{
			case SUCCESS : return "Repeat condition met.";
				break;
			case FAILURE: return "Repeat condition not met.";
				break;
			case RUNNING: return "Repeating until  defined condition met.";
				break;
			default : return "Waiting to run.";
				break;
		}
	}

	int update(float fDeltaTime)
	{
		if (m_nState == RUNNING)
		{
			if (repeat())
			{
				if (!child)
				{
					if (m_vChildren.size() > 0)
						child = m_vChildren[0];
				}

				switch (child->getState())
				{
					case IDLE:
						child->setState(RUNNING);
						break;
					case RUNNING:
						child->update(fDeltaTime);
						break;
					case FAILURE:
						child->setState(IDLE);
						break;
					case SUCCESS:
						child->setState(IDLE);
				}
			}
			else
				m_nState = SUCCESS;
		}

		return m_nState;
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

	BehaviorNode* buildDemoBehaviorTree()
	{
		RepeatDecorator* base = new RepeatDecorator("Root Node", new BehaviorTreeBlackboard());
		base->setState(RUNNING);

		BehaviorNode* node = new SequenceNode();
		base->addChild(node);

		node->addChild(new MouseBehaviorNode());
		
		MoveBehaviorNode* moveA = new MoveBehaviorNode(pEntity);
		node->addChild(moveA);


		return base;
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
		
		bt = buildDemoBehaviorTree();
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