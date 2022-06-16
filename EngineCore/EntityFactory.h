class EntityFactory :
	public BaseNode,
	public EventListener
{
private:
	BaseNode* m_pEntityList;

	void onEvent(_Event* pEvent) {}

public:
	EntityFactory(BaseNode* pEntityList) :
		m_pEntityList(pEntityList),
		BaseNode("EntityFactory")
	{}

	void createPlayer(int playerId)
	{
		string s = "PLAYER ";
		s = s.append(thingToString<int>(playerId + 1));
		BaseNode* pEntity = new BaseNode(s);
		pEntity->addChild(new Render(5));
		pEntity->addChild(new Transform2D({ float(random(1, 10)), float(random(1, 10))}, {0, 0}, {1, 1}));
		pEntity->addChild(new Physics());
		pEntity->addChild(new UserController(playerId));
		pEntity->addChild(new ShootAction(100000, 200.0));
		pEntity->addChild(new Collider());
		m_pEntityList->add(pEntity);

		addEvent(new NewBaseNodeEvent(pEntity));
	}
};