class EntityPhysicsSystem : public BaseNode
{
private:
	BaseNode* m_pEntityList;

public:
	EntityPhysicsSystem(BaseNode* pEntityList) :
		m_pEntityList(pEntityList),
		BaseNode("PhysicsSystem") {}

	void update(float fDeltaTime)
	{
		while (m_pEntityList->isIterating())
		{
			Transform2D* pTransform = m_pEntityList->getCurrent()->getChild<Transform2D>();
			Physics* pPhysics = m_pEntityList->getCurrent()->getChild<Physics>();
			if (pTransform && pPhysics)
			{
				Vector2 vVelocity = pPhysics->getVelocity();
				vVelocity = vVelocity * 0.95;
				if (vVelocity.magnitude() < 0.0001)
					vVelocity = { 0, 0 };

				Vector2 vPosition = pTransform->getPosition();


				pTransform->setPosition(vPosition + (vVelocity * fDeltaTime));
				pPhysics->setVelocity(vVelocity);
			}
		}
	}
};