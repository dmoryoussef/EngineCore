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
			case ACTION_EVENT:
			{
				BaseNode* pParent = pEvent->get<ActionEvent>()->getNode();
				if (Transform2D* transform = pParent->getChild<Transform2D>())
				{
					
					Vector2 forward(transform->getForward().X, transform->getForward().Y);
					Vector2 position = transform->getPosition();

					BaseNode* pEntity = new BaseNode();
					// pEntity->add(new Render());
					pEntity->addChild(new Transform2D(position, forward, { 1, 1 }));
					pEntity->addChild(new Render());
					pEntity->add(new Physics(forward * 1.5));
					m_pEntityList->add(pEntity);
				}
				break;
			}
		}
	}
public:
	EntityFactory(BaseNode* pEntityList) :
		m_pEntityList(pEntityList),
		BaseNode("EntityFactory")
	{
		registerListener(ACTION_EVENT);
	}
};