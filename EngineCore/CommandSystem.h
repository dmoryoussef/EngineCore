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
				_Command* pCommand = pEvent->get<CommandEvent>()->getCommand();
				BaseNode* pParent = pEvent->get<CommandEvent>()->getParent();
				{
					if (pCommand->getType() == ACCELERATE)
					{
						if (Transform2D* pTransform = pParent->getChild<Transform2D>())
						{
							Vector2 vPosition = pTransform->getPosition();
							Vector2 vVelocity = pCommand->get<AccelerateCommand>()->getForce();
							pTransform->setPosition(vPosition + vVelocity);
						}
					}

					if (pCommand->getType() == ROTATE)
					{
						if (Transform2D* pTransform = pParent->getChild<Transform2D>())
						{
							Vector2 vRotate = pCommand->get<RotateCommand>()->getVector();
							pTransform->setRotation(vRotate);
						}
					}

					if (pCommand->getType() == FIRE)
					{
						if (Transform2D* transform = pParent->getChild<Transform2D>())
						{
							if (ShootAction* pShoot = pParent->getChild<ShootAction>())
							{
								if (pShoot->canShoot())
								{

									Vector2 rotation = transform->getRotation();
									Vector2 forward = transform->getForward();
									Vector2 position = transform->getPosition();

									BaseNode* pEntity = new BaseNode();
									// pEntity->add(new Render());
									pEntity->addChild(new Transform2D(position, rotation, { 1, 1 }));
									pEntity->addChild(new Render());
									pEntity->add(new Physics(forward * 1.5));
									m_pEntityList->add(pEntity);
								}
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
	}

public:
	EntityCommandSystem(BaseNode* pEntityList) :
		m_pEntityList(pEntityList),
		BaseNode("Command System") 
	{
		registerListener(COMMAND_EVENT);
	};


};