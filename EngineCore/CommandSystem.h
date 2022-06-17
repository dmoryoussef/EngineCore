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
					//	STEP 1: get relavent component to validate the command can be done
					//	STEP 2: if command is valid - issue execute
					//			seperate systems will process the execute
					// 
					//	for now, everything is done here:

					if (pCommand->getType() == ACCELERATE)
					{
						//	ex:  if entity has an accelerate component:
						//		 execute accelerate force
						//	     physics engine handles

						if (Transform2D* pTransform = pParent->getChild<Transform2D>())
						{
							Vector2 vPosition = pTransform->getPosition();
							Vector2 vVelocity = pCommand->get<AccelerateCommand>()->getForce() * 0.1;
							
							//	let physics engine handle this part
							pTransform->setPosition(vPosition + vVelocity);
						}
					}

					if (pCommand->getType() == ROTATE)
					{
						if (Transform2D* pTransform = pParent->getChild<Transform2D>())
						{
							//	move to physics
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
									
									// move to entity factory??
									//	spawn entity event
									BaseNode* pEntity = new BaseNode();
									pEntity->addChild(new Render(4));
									pEntity->addChild(new Transform2D(position, rotation, { 0.1, 0.1 }));
									pEntity->addChild(new Physics(forward * 0.5));
									pEntity->addChild(new Collider());
									m_pEntityList->add(pEntity);

									addEvent(new NewBaseNodeEvent(pEntity));
								}
							}
						}
					}

					delete pCommand;

					break;
				}
			}
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