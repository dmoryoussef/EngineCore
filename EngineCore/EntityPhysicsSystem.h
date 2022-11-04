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
			{
				vTotal = vTotal + accel->getForce();
				//	accel->setForce({0, 0});
			}

			Accelerate* pAccel = pEntity->getChild<Accelerate>();
			Transform2D* pTransform = pEntity->getChild<Transform2D>();
			Velocity* pVelocity = pEntity->getChild<Velocity>();

			if (pTransform && pVelocity)
			{
				Vector2 vVelocity = pVelocity->getVelocity();

				//	add acceleration forces
				vVelocity = vVelocity + vTotal;
				
				//	add friction
				vVelocity = vVelocity * 0.972;
				if (vVelocity.magnitude() < 0.00001)
					vVelocity = vVelocity * 0.0;

				//	set back
				pVelocity->setVelocity(vVelocity);

				//	set new position
				Vector2 vPosition = pTransform->getPosition();
				vVelocity = vVelocity * fDeltaTime;

				pTransform->setRotation({ -vVelocity.X, vVelocity.Y });
				pTransform->setPosition(vPosition + vVelocity);
			}
		}
	}
};