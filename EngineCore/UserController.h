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
					float LeftStickX = Controller.LeftStickX;
					float LeftStickY = Controller.LeftStickY;

					Vector2 vVelocity(LeftStickX, -LeftStickY);
					vVelocity = vVelocity * 0.1;
					//	COMMAND -> accelerate
			
					float RightStickX = Controller.RightStickX;
					float RightStickY = Controller.RightStickY;
					if (RightStickX != 0.0 || RightStickY != 0.0)
					{
						//	only update rotation of stick is moved
						//	otherwise it resets to 0
						//	COMMAND -> rotate

					}

					if (pEvent->get<GamePadEvent>()->getState().XButton)
					{
						//	COMMAND --> ACTION
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