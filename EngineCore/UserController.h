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

class UserController : public _EntityComponent
{
private:
	int m_nControllerID;

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
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
						addEvent(new CommandEvent(m_nControllerID, new AccelerateCommand(vVelocity)));
						
					}

					if (Controller.RightStickX != 0.0 || Controller.RightStickY != 0.0)
					{
						//	only update rotation of stick is moved
						//	otherwise it resets to 0
						//	COMMAND -> rotate
						Vector2 vRotate(-Controller.RightStickX, -Controller.RightStickY);
						addEvent(new CommandEvent(m_nControllerID, new RotateCommand(vRotate)));

					}

					if (Controller.XButton)
					{
						//	COMMAND --> ACTION
						//addEvent(new CommandEvent(m_nControllerID, FIRE));
					}
				}
				
				break;
			}
		}
	}

public:
	UserController(int id) :
		m_nControllerID(id),
		_EntityComponent("USER_CONTROLLER")
	{
		registerListener(GAMEPAD_EVENT);
	}

	int getControllerId()
	{
		return m_nControllerID;
	}
};