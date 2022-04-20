class EntityPhysicsSystem : public BaseNode
{
private:

public:
	EntityPhysicsSystem() :
		BaseNode() {}

	void update(BaseNode *pEntities, float fDeltaTime)
	{
		while (pEntities->isIterating())
		{
			Transform2D* pTransform = pEntities->getCurrent()->getChild<Transform2D>();
			Physics* pPhysics = pEntities->getCurrent()->getChild<Physics>();
			if (pTransform && pPhysics)
			{
				Vector2 vVelocity = pPhysics->getVelocity();
				Vector2 vPosition = pTransform->getPosition();
				pTransform->setPosition(vPosition + vVelocity);
			}
		}
	}
};