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
			BaseNode* pEntity = m_pEntityList->getCurrent();

			vector<Accelerate*> AccelChildren = pEntity->getChildren<Accelerate>();
			Vector2 vTotal;
			for (auto accel : AccelChildren)
				vTotal = vTotal + accel->getForce();

			Transform2D* pTransform = pEntity->getChild<Transform2D>();
			Velocity* pVelocity = pEntity->getChild<Velocity>();

			if (pTransform && pVelocity)
			{
				Vector2 vVelocity = pVelocity->getVelocity();
				vVelocity = vVelocity + vTotal;
					
				vVelocity = vVelocity * 0.9;
				if (vVelocity.magnitude() < 0.00001)
					vVelocity = { 0, 0 };

				pVelocity->setVelocity(vVelocity);

				Vector2 vPosition = pTransform->getPosition();
				pTransform->setPosition(vPosition + (vVelocity * fDeltaTime));
			}
		}
	}
};