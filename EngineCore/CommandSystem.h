class EntityCommandSystem : public BaseNode, EventListener
{
private:
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

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case COMMAND_EVENT:
			{
				_Command *pCommand = pEvent->get<CommandEvent>()->getCommand();
				if (BaseNode *pEntity = getEntity(pEvent->get<CommandEvent>()->getId()))
				{
					//	entity has userController with matching id
					//	SO
					//	do command
					switch (pCommand->getType())
					{
						case ACCELERATE:
						{
							if (Transform2D* pTransform = pEntity->getChild<Transform2D>())
							{
								Vector2 vPosition = pTransform->getPosition();
								Vector2 vVelocity = pCommand->get<AccelerateCommand>()->getForce();
								pTransform->setPosition(vPosition + vVelocity);
							}
							
							break;
						}

						case ROTATE:
						{
							if (Transform2D* pTransform = pEntity->getChild<Transform2D>())
							{
								Vector2 vRotate = pCommand->get<RotateCommand>()->getVector();
								pTransform->setRotation(vRotate);

							}
							break;
						}
					}
				}
				delete pCommand;

				break;
			}
		}
		//	case command event
		//		shootAction:
		//			get action->execute()
		//	case accelerate command
		//		get transform, physics
		//			add accelerate force
	}

public:
	EntityCommandSystem(BaseNode* pEntityList) :
		m_pEntityList(pEntityList),
		BaseNode("Command System") 
	{
		registerListener(COMMAND_EVENT);
	};


};