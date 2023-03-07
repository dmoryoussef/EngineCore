

//class UserController : public _EntityComponent
//{
//private:
//	int m_nControllerID;
//
//	bool bUp;
//	bool bDown;
//	bool bLeft;
//	bool bRight;
//
//
//	void onMouseWorldEvent(MouseWorldEvent* pEvent)
//	{
//		if (pEvent->getState().bRightButtonDown)
//		{
//			Vector2 vTarget = pEvent->getWorldPosition();
//			addEvent(new CommandEvent(getParent(), new ActionCommand(vTarget)));
//		}
//
//		/*Vector2 vRotate(pEvent->getWorldPosition());
//		addEvent(new CommandEvent(getParent(), new RotateCommand(vRotate)));*/
//
//	}
//	void onKeyboardEvent(KeyboardEvent* pEvent)
//	{
//		if (m_nControllerID == 0)
//		{
//			if (pEvent->getKey() == 'W' || pEvent->getKey() == 'w')
//				bUp = pEvent->isKeyDown();
//
//			if (pEvent->getKey() == 'S' || pEvent->getKey() == 's')
//				bDown = pEvent->isKeyDown();
//			
//
//			if (pEvent->getKey() == 'D' || pEvent->getKey() == 'd')
//				bRight = pEvent->isKeyDown();
//			
//			
//			if (pEvent->isKeyDown('a'))
//				bLeft = true;
//					
//			if (pEvent->isKeyUp('a'))
//				bLeft = false;
//
//
//			if (pEvent->isKeyDown(32))
//				addEvent(new CommandEvent(getParent(), new ActionCommand()));
//		} 
//	}
//	void onGamepadEvent(GamePadEvent* pEvent)
//	{
//		if (pEvent->getState().nIndex == m_nControllerID)
//		{
//			GamePad Controller = pEvent->getState();
//
//			//	Translate raw input into a specific command for a specific game object
//			//	
//			//	Individual components for listening for each specific command 
//			//	OR
//			//	Single system to handle
//
//
//			if (Controller.LeftStickX != 0.0 || Controller.LeftStickY != 0.0)
//			{
//				Vector2 vVelocity(Controller.LeftStickX, -Controller.LeftStickY);
//				vVelocity = vVelocity * 0.1;
//				addEvent(new CommandEvent(getParent(), new AccelerateCommand(vVelocity)));
//
//			}
//
//			if (Controller.RightStickX != 0.0 || Controller.RightStickY != 0.0)
//			{
//				//	only update rotation of stick is moved
//				//	otherwise it resets to 0
//				//	COMMAND -> rotate
//				Vector2 vRotate(-Controller.RightStickX, -Controller.RightStickY);
//				addEvent(new CommandEvent(getParent(), new RotateCommand(vRotate)));
//
//			}
//
//			if (Controller.RightTrigger > 0.0)
//			{
//				//	COMMAND --> ACTION
//				addEvent(new CommandEvent(getParent(), new ActionCommand()));
//			}
//		}
//	}
//	void onEvent(_Event* pEvent)
//	{
//		switch (pEvent->m_eType)
//		{
//			case MOUSEWORLD_EVENT:	onMouseWorldEvent(pEvent->get<MouseWorldEvent>());
//				break;
//			case KEYBOARD_EVENT:	onKeyboardEvent(pEvent->get<KeyboardEvent>());
//				break;
//			case GAMEPAD_EVENT:		onGamepadEvent(pEvent->get<GamePadEvent>());
//				break;
//		}
//	}
//
//
//public:
//	UserController(int id) :
//		m_nControllerID(id),
//		_EntityComponent("USER_CONTROLLER")
//	{
//		registerListener(GAMEPAD_EVENT);
//		registerListener(MOUSEWORLD_EVENT);
//		registerListener(KEYBOARD_EVENT);
//	}
//	void update(float fDeltaTime)
//	{
//		Vector2 vVelocity(0, 0);
//		if (bUp)	vVelocity = vVelocity + Vector2(0, -1);
//		if (bLeft)	vVelocity = vVelocity + Vector2(-1, 0);
//		if (bDown)	vVelocity = vVelocity + Vector2(0, 1);
//		if (bRight) vVelocity = vVelocity + Vector2(1, 0);
//		if (vVelocity.magnitude() != 0)
//			addEvent(new CommandEvent(getParent(), new AccelerateCommand(vVelocity)));
//	}
//
//	int getControllerId()
//	{
//		return m_nControllerID;
//	}
//};

class UserController : public _EntityComponent
{
private:
	int m_nControllerID;

	bool bUp;
	bool bDown;
	bool bLeft;
	bool bRight;
	bool bAction;


	bool bRotateLeft;
	bool bRotateRight;
	bool bForward;
	bool bBack;

	bool bDirectScheme;

	void onMouseWorldEvent(MouseWorldEvent* pEvent)
	{
		if (pEvent->getState().bRightButtonDown)
		{
			Vector2 vTarget = pEvent->getWorldPosition();
			addEvent(new CommandEvent(getParent(), new ActionCommand(vTarget)));
		}

		/*Vector2 vRotate(pEvent->getWorldPosition());
		addEvent(new CommandEvent(getParent(), new RotateCommand(vRotate)));*/

	}
	void onKeyboardEvent(KeyboardEvent* pEvent)
	{
		if (m_nControllerID == 0)
		{
			if (bDirectScheme)
			{
				//	Control scheme with direct movement mapped to keys
				if (pEvent->getKey() == 'W' || pEvent->getKey() == 'w')	bForward = pEvent->isKeyDown();
				if (pEvent->getKey() == 'S' || pEvent->getKey() == 's')	bBack = pEvent->isKeyDown();
				if (pEvent->getKey() == 'D' || pEvent->getKey() == 'd')	bRight = pEvent->isKeyDown();
				if (pEvent->isKeyDown('a'))	bLeft = true;
				if (pEvent->isKeyUp('a')) bLeft = false;
			}
			else
			{
				//	Alternate scheme with left/right mapped to rotate, and up/down for forward/reverse
				if (pEvent->getKey() == 'W' || pEvent->getKey() == 'w')	bUp = pEvent->isKeyDown();
				if (pEvent->getKey() == 'S' || pEvent->getKey() == 's')	bDown = pEvent->isKeyDown();
				if (pEvent->getKey() == 'D' || pEvent->getKey() == 'd')	bRotateRight = pEvent->isKeyDown();
				if (pEvent->isKeyDown('a'))	bRotateLeft = true;
				if (pEvent->isKeyUp('a')) bRotateLeft = false;
			}
			
			if (pEvent->isKeyDown(32)) bAction = true;
			if (pEvent->isKeyUp(32)) bAction = false;
		}
	}
	void onGamepadEvent(GamePadEvent* pEvent)
	{
		if (pEvent->getState().nIndex == m_nControllerID)
		{
			GamePad Controller = pEvent->getState();

			//	Translate raw input into a specific command for a specific game object
			//	
			//	Individual components for listening for each specific command 
			//	OR
			//	Single system to handle


			if (Controller.LeftStickX != 0.0 || Controller.LeftStickY != 0.0)
			{
				Vector2 vAccelerate(Controller.LeftStickX, -Controller.LeftStickY);
				vAccelerate = vAccelerate * 0.01;
				if (Accelerate* accel = getParent()->getChild<Accelerate>())
				{
					accel->setForce(vAccelerate);
				}

			}

			if (Controller.RightStickX != 0.0 || Controller.RightStickY != 0.0)
			{
				//	only update rotation of stick is moved
				//	otherwise it resets to 0
				//	COMMAND -> rotate
				Vector2 vRotate(-Controller.RightStickX, -Controller.RightStickY);
				

			}

			if (Controller.RightTrigger > 0.0)
			{
				//	COMMAND --> ACTION
				
			}
		}
	}
	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case MOUSEWORLD_EVENT:	onMouseWorldEvent(pEvent->get<MouseWorldEvent>());
				break;
			case KEYBOARD_EVENT:	onKeyboardEvent(pEvent->get<KeyboardEvent>());
				break;
			case GAMEPAD_EVENT:		onGamepadEvent(pEvent->get<GamePadEvent>());
				break;
		}
	}

	void setForce(BaseNode *pParent, Vector2 v)
	{
		if (pParent)
		{
			for (BaseNode* pCurrent = pParent->getStart(); pCurrent != NULL; pCurrent = pCurrent->getNext())
			{
				if (typeid(Accelerate) == typeid(*pCurrent))
				{
					Accelerate* pAccel = static_cast<Accelerate*>(pCurrent);
					float scaler = pAccel->getMax();
					pAccel->setForce(v * scaler);
				}
				setForce(pCurrent, v);
			}
		}
	}

public:
	UserController(int id) :
		m_nControllerID(id),
		bDirectScheme(false),
		bUp(false),
		bDown(false),
		bLeft(false),
		bRight(false),
		bAction(false),
		bForward(false),
		bBack(false),
		bRotateLeft(false),
		bRotateRight(false),
		_EntityComponent("USER_CONTROLLER")
	{
		registerListener(GAMEPAD_EVENT);
		registerListener(MOUSEWORLD_EVENT);
		registerListener(KEYBOARD_EVENT);
	}

	void update(float fDeltaTime)
	{
		if (bAction)
		{
			if (ShootAction* pAction = getParent()->getChild<ShootAction>())
			{
				pAction->tryAction();
			}
		} 

		Vector2 vForce(0, 0);
		if (bUp)	vForce = vForce + Vector2(0, -1);
		if (bLeft)	vForce = vForce + Vector2(-1, 0);
		if (bDown)	vForce = vForce + Vector2(0, 1);
		if (bRight) vForce = vForce + Vector2(1, 0);

		if (bForward)		vForce = getParent()->getChild<Transform2D>()->getForward().normalize();	// not safe
		if (bBack)			vForce = -getParent()->getChild<Transform2D>()->getForward().normalize();
		if (bRotateLeft)	getParent()->getChild<Transform2D>()->rotate(0);
		if (bRotateRight)	getParent()->getChild<Transform2D>()->rotate(0);

		setForce(getParent(), vForce);
	}

	int getControllerId()
	{
		return m_nControllerID;
	}
};