class EntityFactory :
	public BaseNode,
	public EventListener
{
private:
	BaseNode* m_pEntityList;

	BaseNode* createParticle(Vector2 position)
	{
		BaseNode* pEntity = new BaseNode("Particle");
		pEntity->addNode(new Render(3))->
				 addNode(new Transform2D(position, { 0, 0 }, { .5, .5 }))->
				 addNode(new Velocity(Vector2(float(random(-1, 1)), float(random(-1, 1)) * .001)));
		return pEntity;
	}

	BaseNode* createProjectile(Vector2 position, Vector2 rotation, Vector2 forward)
	{
		BaseNode* pEntity = new BaseNode("Projectile");
		pEntity->addNode(new Render(Polygon2D(2, 0.8), { PIXEL_HALF, FG_LIGHTRED }))->
				 addNode(new Transform2D(position, rotation, { 1, 1 }))->
				 addNode(new Velocity(forward * 0.09, false))->
				 addNode(new OutOfBoundsCollision(Vector2(-250, -250), Vector2(250, 250)))->
				 addChild(new Collider2D(Polygon2D(2, 0.8)));
		//pEntity->addChild(new CollisionDamage(25));
		return pEntity;
	}

	//	change to action event???A!@
	void onCommandEvent(CommandEvent* pEvent)
	{
		BaseNode* pParent = pEvent->getParent();
		_Command* pCommand = pEvent->getCommand();
		if (pCommand->getType() == FIRE)
		{
			if (Transform2D* transform = pParent->getChild<Transform2D>())
			{
				if (ShootAction* pShoot = pParent->getChild<ShootAction>())
				{
					Vector2 rotation = transform->getRotation();
					Vector2 forward = transform->getForward().normalize();
					Vector2 position = transform->getPosition() + (forward * 1.5);
					m_pEntityList->add(createProjectile(position, rotation, forward));
					//	addEvent(new NewBaseNodeEvent(pEntity));
				}
			}

			delete pCommand;
		}
	}

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case DELETE_BASENODE_EVENT:
			{
				BaseNode* pDelete = pEvent->get<DeleteBaseNodeEvent>()->getNode();
				//	Do special on delete stuff first
				//	pDelete->onDelete();
				if (pDelete->getName() == "PLAYER 1" || pDelete->getName() == "PLAYER 2")
				{
					//	explode
					for (int i = 0; i < 3; i++)
					{
						m_pEntityList->add(createParticle(pDelete->getChild<Transform2D>()->getPosition()));
					}
				}
				
				m_pEntityList->remove(pDelete);
				
				break;
			}
			case COMMAND_EVENT: onCommandEvent(pEvent->get<CommandEvent>());
				break;

		}
	}

public:
	EntityFactory(BaseNode* pEntityList) :
		m_pEntityList(pEntityList),
		BaseNode("EntityFactory")
	{
		registerListener(DELETE_BASENODE_EVENT);
		registerListener(COMMAND_EVENT);
	}

	BaseNode *createPlayer(int playerId)
	{
		string s = "PLAYER ";
		s = s.append(thingToString<int>(playerId + 1));
		Polygon2D shape; 
		shape.createPoly(3);
		BaseNode* pEntity = new BaseNode(s);
		pEntity->addChild(new Render(shape));
		pEntity->addChild(new Transform2D({ float(random(1, 10)), float(random(1, 10))}, {1, 0}, {1, 1}));
		pEntity->addChild(new Velocity(1.0f));
		pEntity->addChild(new UserController(playerId, DIRECT));
		pEntity->addChild(new ShootAction(100000, 200.0));
		pEntity->addChild(new Collider2D(shape));
		pEntity->addChild(new UIState()); 
		pEntity->addChild(new Health(100));
		pEntity->addChild(new Accelerate(0.0));
		
		addEvent(new NewBaseNodeEvent(pEntity));

		return pEntity;
	}
};