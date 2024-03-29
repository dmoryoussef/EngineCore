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
		pEntity(entity),
		LeafNode("Move")
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



class BehaviorTreeDemo : public GameState
{
private:
	BehaviorNode* bt;
	Transform3D* pCameraTransform;

	BaseNode* pEntityA;
	BaseNode* pEntityB;

	BehaviorNode* TreeA;
	BehaviorNode* TreeB;

public:
	BehaviorTreeDemo() :
		bt(NULL),
		pEntityA(NULL),
		pEntityB(NULL),
		pCameraTransform(NULL) {};

	//void buildDemoBehaviorTree(BaseNode *entity)
	//{
	//	RepeatDecorator* base = new RepeatDecorator("Root Node", new BehaviorTreeBlackboard(entity));
	//	base->setState(RUNNING);

	//	/*BehaviorNode* sequence = new SequenceNode();
	//	base->addChild(sequence);*/

	//	base->addChild(new PursuitBehaviorNode());

	//	//BehaviorNode* parallel = new ParallelNode();
	//	// sequence->addChild(parallel);
	//	//	sequence->addChild(new FleeBehavior());
	//	//node->addChild(new MouseBehaviorNode());
	//	//MoveBehaviorNode* moveA = new MoveBehaviorNode(pEntity);
	//	//node->addChild(moveA);
	//	//	parallel->addChild(new SeekBehaviorNode(pEntity));
	//	//	parallel->addChild(new AttackBehavior(pEntity));

	//	entity->addChild(new Behavior(base));
	//}

	void start(BaseNode* pData, BaseNode* pSystems, BaseNode* pGUI)
	{
		int height = pGUI->cast<_UIComponent>()->getHeight();
		int width = pGUI->cast<_UIComponent>()->getWidth();
		CameraViewWindow* pCameraWindow = new CameraViewWindow(width, height, 0, 0);
		BaseNode* pCamera = pCameraWindow->getCamera();
		pCameraTransform = pCamera->getChild<Transform3D>();
		//pData->add(pCamera);
		pGUI->addAtEnd(pCameraWindow);

		pEntityA = pSystems->getChild<EntityFactory>()->createPlayer(0);
		pData->add(pEntityA);

		pEntityB = pSystems->getChild<EntityFactory>()->createPlayer(1);
		pData->add(pEntityB);
		
		RepeatDecorator* baseA = new RepeatDecorator("Root Node", new BehaviorTreeBlackboard(pEntityA, pData));
		baseA->setState(RUNNING);
		BehaviorNode* sequenceA = new SequenceNode();
		baseA->addNode(sequenceA);
		sequenceA->addNode(new GetTarget());
		sequenceA->addNode(new PursuitBehavior());
		pEntityA->addChild(new Behavior(baseA));

		RepeatDecorator* baseB = new RepeatDecorator("Root Node", new BehaviorTreeBlackboard(pEntityB, pData));
		baseB->setState(RUNNING);
		BehaviorNode* sequenceB = new SequenceNode();
		baseB->addNode(sequenceB);
		sequenceB->addNode(new GetTarget());
		sequenceB->addNode(new FleeBehavior());
		pEntityB->addChild(new Behavior(baseB));

		pEntityA->getChild<Transform2D>()->setPosition({ 40, 20 });

		TreeA = baseA;
		TreeB = baseB;
	}

	void update(BaseNode* pData, float fDeltaTime) {}

	void render(OutputBuffer* pEngineBuffer)
	{
		Render2D renderer(pEngineBuffer, pCameraTransform->getPosition());
		
		TreeA->render(&renderer, 5, 5);
		TreeB->render(&renderer, 25, 5);

	}
};