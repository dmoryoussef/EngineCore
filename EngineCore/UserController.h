enum COMMANDS
{
	ACCELERATE,
	ROTATE,
	FIRE
};

class AccelerateCommand : public _Command
{
private:
	Vector2 m_vForce;

public:
	AccelerateCommand(Vector2 f) :
		m_vForce(f),
		_Command(ACCELERATE) {};

	Vector2 getForce() { return m_vForce; }
};

class RotateCommand : public _Command
{
private: 
	Vector2 m_vRotate;

public:
	RotateCommand(Vector2 r) :
		m_vRotate(r),
		_Command(ROTATE) {};

	Vector2 getVector() { return m_vRotate; }
};

class ActionCommand : public _Command
{
private:
public:
	ActionCommand() :
		_Command(FIRE) {};
};

void test()
{
	cout << "from global function \n";
}

class UserController : public _EntityComponent
{
private:
	int m_nControllerID;

	bool bUp;
	bool bDown;
	bool bLeft;
	bool bRight;

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case CONSOLE_KEYBOARD_EVENT:
			{
				if (m_nControllerID == 0)
				{
					KeyboardEvent* pKeyBoardEvent = pEvent->get<KeyboardEvent>();

					if (pKeyBoardEvent->getKey() == 'W' || pKeyBoardEvent->getKey() == 'w')
					{
						bUp = pKeyBoardEvent->isKeyDown();
					}

					if (pKeyBoardEvent->getKey() == 'A' || pKeyBoardEvent->getKey() == 'a') 
					{
						bLeft = pKeyBoardEvent->isKeyDown();
					}

					if (pKeyBoardEvent->getKey() == 'S' || pKeyBoardEvent->getKey() == 's')
					{
						bDown = pKeyBoardEvent->isKeyDown();
					}

					if (pKeyBoardEvent->getKey() == 'D' || pKeyBoardEvent->getKey() == 'd')
					{
						bRight = pKeyBoardEvent->isKeyDown();
					}
			
				} 
				break;
			}
			case GAMEPAD_EVENT:
			{
				GamePadEvent* pGPE = pEvent->get<GamePadEvent>();
				if (pGPE->getState().nIndex == m_nControllerID)
				{
					GamePad Controller = pEvent->get<GamePadEvent>()->getState();
					
					//	Translate raw input into a specific command for a specific game object
					//	
					//	Individual components for listening for each specific command 
					//	OR
					//	Single system to handle
				

					if (Controller.LeftStickX != 0.0 || Controller.LeftStickY != 0.0)
					{
						Vector2 vVelocity(Controller.LeftStickX, -Controller.LeftStickY);
						vVelocity = vVelocity * 0.1;
						addEvent(new CommandEvent(getParent(), new AccelerateCommand(vVelocity)));
						
					}

					if (Controller.RightStickX != 0.0 || Controller.RightStickY != 0.0)
					{
						//	only update rotation of stick is moved
						//	otherwise it resets to 0
						//	COMMAND -> rotate
						Vector2 vRotate(-Controller.RightStickX, -Controller.RightStickY);
						addEvent(new CommandEvent(getParent(), new RotateCommand(vRotate)));

					}

					if (Controller.RightTrigger > 0.0)
					{
						//	COMMAND --> ACTION
						addEvent(new CommandEvent(getParent(), new ActionCommand()));
					}
				}
				
				break;
			}
		}
	}

	void update(float fDeltaTime)
	{
		Vector2 vVelocity(0, 0);
		if (bUp) vVelocity = vVelocity + Vector2(0, -1);
		if (bLeft) vVelocity = vVelocity + Vector2(-1, 0);
		if (bDown) vVelocity = vVelocity + Vector2(0, 1);
		if (bRight) vVelocity = vVelocity + Vector2(1, 0);
		if (vVelocity.magnitude() != 0)
			addEvent(new CommandEvent(getParent(), new AccelerateCommand(vVelocity)));
	}

public:
	UserController(int id) :
		m_nControllerID(id),
		_EntityComponent("USER_CONTROLLER")
	{
		registerListener(GAMEPAD_EVENT);
		registerListener(CONSOLE_KEYBOARD_EVENT);
	}


	int getControllerId()
	{
		return m_nControllerID;
	}
};