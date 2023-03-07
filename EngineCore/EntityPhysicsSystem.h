class EntityPhysicsSystem : public BaseNode
{
private:
	BaseNode* m_pEntityList;
	Vector2 getForce(BaseNode* pParent)
	{
		Vector2 vForce;
		if (pParent)
		{
			for (BaseNode* pCurrent = pParent->getStart(); pCurrent != NULL; pCurrent = pCurrent->getNext())
			{
				if (typeid(Accelerate) == typeid(*pCurrent))
				{
					Accelerate* pAccel = static_cast<Accelerate*>(pCurrent);
					vForce = vForce + pAccel->getForce();
				}
				vForce = vForce + getForce(pCurrent);
			}
		}
		return vForce;
	}
public:
	EntityPhysicsSystem(BaseNode* pEntityList) :
		m_pEntityList(pEntityList),
		BaseNode("PhysicsSystem") {}

	void update(float fDeltaTime)
	{
		while (m_pEntityList->isIterating())
		{
			BaseNode* pEntity = m_pEntityList->getCurrent();

			Vector2 vTotal = getForce(pEntity);
			vTotal = vTotal * fDeltaTime;

			Accelerate* pAccel = pEntity->getChild<Accelerate>();
			Transform2D* pTransform = pEntity->getChild<Transform2D>();
			Velocity* pVelocity = pEntity->getChild<Velocity>();

			if (pTransform && pVelocity)
			{
				Vector2 vVelocity = pVelocity->getVelocity();

				//	add acceleration forces
				vVelocity = vVelocity + vTotal;
				
				//	add friction
				if (pVelocity->applyFriction())
					vVelocity = vVelocity * 0.995;

				//	clamp to 0 if magnitude is less than
				vVelocity.floor(0.0001);

				pVelocity->setVelocity(vVelocity);


				//	set back

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