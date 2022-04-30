enum COMMANDS
{
	ACCEL,
	ROTATE,
	FIRE
};

class _Command
{
private:
	int m_id;

public:
	_Command(int id) :
		m_id(id) {};

	int getId() { return m_id; }
};

class AccelerateCommand : public _Command
{
private:
	Vector2 m_vForce;

public:
	AccelerateCommand(Vector2 f) :
		m_vForce(f),
		_Command(ACCEL) {};

	Vector2 getForce() { return m_vForce; }
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
				if (pEvent->get<GamePadEvent>()->getState().nIndex == m_nControllerID)
				{
					GamePad Controller = pEvent->get<GamePadEvent>()->getState();
					
					if (Controller.LeftStickX != 0.0 || Controller.LeftStickY != 0.0)
					{
						Vector2 vVelocity(Controller.LeftStickX, -Controller.LeftStickY);
						vVelocity = vVelocity * 0.1;
						addEvent(new CommandEvent(m_nControllerID, new AccelerateCommand(vVelocity));
						
					}

					if (Controller.RightStickX != 0.0 || Controller.RightStickY != 0.0)
					{
						//	only update rotation of stick is moved
						//	otherwise it resets to 0
						//	COMMAND -> rotate
						//addEvent(new CommandEvent(m_nControllerID, ROTATE));

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
		_EntityComponent("USER_CONTROLLER") {}
};