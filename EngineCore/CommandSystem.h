class EntityCommandSystem : public BaseNode, EventListener
{
private:
	//	This entire system should be completely replaced by components handling their own commands
	//	for now everthing is lumped here to test functionality


	BaseNode* m_pEntityList;

	BaseNode* getEntity(int id)
	{
		while (m_pEntityList->isIterating())
		{
			BaseNode* pEntity = m_pEntityList->getCurrent();
			if (UserController* pController = pEntity->getChild<UserController>())
			{
				if (pController->getControllerId() == id)
				{
					return pEntity;
				}
			}

		}
		return NULL;
	}

	void onCommandEvent(CommandEvent *pEvent)
	{
		//	STEP 1: get relavent component to validate the command can be done
		//	STEP 2: if command is valid - issue execute
		//			seperate systems will process the execute
		// 
		//	for now, everything is done here:
		BaseNode* pParent = pEvent->getParent();
		_Command* pCommand = pEvent->getCommand();
		if (pCommand->getType() == ACCELERATE)
		{
			//	ex:  if entity has an accelerate component:
			//		 execute accelerate force
			//	     physics engine handles

			if (Velocity* pPhysics = pParent->getChild<Velocity>())
			{
				Vector2 vVelocity = pPhysics->getVelocity();

				//	get thruster force
				//	acceleration = entity thruster force * controller magnitude * constant
				Vector2 vAcceleration = pCommand->get<AccelerateCommand>()->getForce() * 0.01;

				Vector2 vNewVelocity = vVelocity + vAcceleration;
				pPhysics->setVelocity(vNewVelocity);
			}
			delete pCommand;
		}

		if (pCommand->getType() == ROTATE)
		{
			if (Transform2D* pTransform = pParent->getChild<Transform2D>())
			{
				//	move to physics?
				Vector2 vRotate = pCommand->get<RotateCommand>()->getVector();
				pTransform->setRotation(vRotate);
			}
			delete pCommand;
		}
			

	}

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{ 
			case COMMAND_EVENT: onCommandEvent(pEvent->get<CommandEvent>());
				break;
			case MOUSEWORLD_EVENT: //	onMouseWorldEvent(pEvent->get<MouseWorldEvent>());
				break;
		}
	}

public:
	EntityCommandSystem(BaseNode* pEntityList) :
		m_pEntityList(pEntityList),
		BaseNode("Command System") 
	{
		registerListener(COMMAND_EVENT);
	};


};