class Behavior : public _EntityComponent
{
private:
	//	default behavior "move to location"
	int behaviorType;
	Vector2 vTarget;

	void onCommandEvent(CommandEvent* pEvent)
	{
		if (ActionCommand* pCommand = pEvent->getCommand()->get<ActionCommand>())
		{
			vTarget = pCommand->getTarget();
		}

	}

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case COMMAND_EVENT: onCommandEvent(pEvent->get<CommandEvent>());
				break;
		}
	}
public:
	Behavior() :
		_EntityComponent("BEHAVIOR")
	{
		registerListener(COMMAND_EVENT);
	};

	void update()
	{
		//	if target destination != current location
		//		step towards target
	}

};