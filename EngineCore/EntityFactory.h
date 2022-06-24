class EntityFactory :
	public BaseNode,
	public EventListener
{
private:
	BaseNode* m_pEntityList;

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case DELETE_BASENODE_EVENT:
			{
				BaseNode* pDelete = pEvent->get<DeleteBaseNodeEvent>()->getNode();


				//	Do special on delete stuff first
				//	pDelete->onDelete();
				//if (pDelete->getName() == "PLAYER 1" || pDelete->getName() == "PLAYER 2")
				//{
				//	//	explode
				//	for (int i = 0; i < 3; i++)
				//	{
				//		BaseNode* pEntity = new BaseNode();
				//		pEntity->addChild(new Render(3));
				//		pEntity->addChild(new Transform2D(pDelete->getChild<Transform2D>()->getPosition(), {0, 0}, {.5, .5}));
				//		pEntity->addChild(new Physics(Vector2(float(random(-1, 1)), float(random(-1, 1)) * .001)));
				//		m_pEntityList->add(pEntity);
				//	}
				//}
				
				m_pEntityList->remove(pDelete);
				
				break;
			}
		}
	}

public:
	EntityFactory(BaseNode* pEntityList) :
		m_pEntityList(pEntityList),
		BaseNode("EntityFactory")
	{
		registerListener(DELETE_BASENODE_EVENT);
	}

	void createPlayer(int playerId)
	{
		string s = "PLAYER ";
		s = s.append(thingToString<int>(playerId + 1));
		BaseNode* pEntity = new BaseNode(s);
		pEntity->addChild(new Render(3));
		pEntity->addChild(new Transform2D({ float(random(1, 10)), float(random(1, 10))}, {0, 0}, {1, 1}));
		pEntity->addChild(new Physics());
		pEntity->addChild(new UserController(playerId));
		pEntity->addChild(new ShootAction(100000, 200.0));
		pEntity->addChild(new Collider());
		pEntity->addChild(new UIState()); 
		pEntity->addChild(new Health(100));
		m_pEntityList->add(pEntity);

		addEvent(new NewBaseNodeEvent(pEntity));
	}
};