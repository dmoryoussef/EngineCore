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
				Vector2 vForce = accel->getForce();
				vTotal = vTotal + vForce;
				//accel->setForce(Vector2(0, 0));
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

				//	clamp to 0 if magnitude is less than
				vVelocity.floor(0.0001);


				//	set back
				pVelocity->setVelocity(vVelocity);

				//	set new position
				Vector2 vPosition = pTransform->getPosition();
				vVelocity = vVelocity * fDeltaTime;

				if (vVelocity.X == 0 && vVelocity.Y == 0)
				{
					// dont update rotation
				}else
					pTransform->setRotation(Vector2(-vVelocity.X, vVelocity.Y));
				pTransform->setPosition(vPosition + vVelocity);
			}
		}
	}
};