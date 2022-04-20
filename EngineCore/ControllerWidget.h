class ControllerWidget :
	public _EntityComponent
{
private:
	Vector2 Position;
	Vector2 StickPosition;

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case GAMEPAD_EVENT:
			{
				StickPosition.X = pEvent->get<GamePadEvent>()->getState().LeftStickX;
				StickPosition.Y = pEvent->get<GamePadEvent>()->getState().LeftStickY;
				break;
			}
		}
	}

public:
	ControllerWidget() :
		Position(5, 5),
		_EntityComponent("ControllerWidget") 
	{
		registerListener(GAMEPAD_EVENT);
	}


	void render(Render2D* pRenderer, Vector3 vCameraPosition, Vector2 vWorldMin, Vector2 vWorldMax)
	{
		//pRenderer->DrawString(StickPosition.toString<float>(), Position.X, Position.Y);

	}
};