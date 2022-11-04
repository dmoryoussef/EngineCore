class FleeBehavior : public LeafNode
{
private:
	BaseNode* m_pSelf;

public:
	FleeBehavior(BaseNode *pSelf) :
		m_pSelf(pSelf) {};

	int execute(float fDeltaTime)
	{
		Vector2 vTarget;
		Vector2 vPosition;
		Vector2 vDesiredDirection;
		Vector2 vVelocity;
		Vector2 vSteer;
		Vector2 vAccel;

		if (m_pSelf == NULL)
		{
			return FAILURE;
		}

		if (Transform2D* pTransform = m_pSelf->getChild<Transform2D>())
		{
			if (Accelerate* accel = m_pSelf->getChild<Accelerate>())
			{
				if (Velocity* pVelocity = m_pSelf->getChild<Velocity>())
				{
					vTarget = Blackboard->vTarget;
					vPosition = pTransform->getPosition();
					float fDistance = distance(vTarget, vPosition);
					if (fDistance < 25.0)
					{
						vDesiredDirection = -(vTarget - vPosition).normalize();

						vVelocity = pVelocity->getVelocity();
						vSteer = (vDesiredDirection - vVelocity).normalize();

						float fThrust = accel->getMax();
						vAccel = vSteer * fThrust;

						accel->setForce(vAccel);

						return RUNNING;	//	not at target yet
					}
					return SUCCESS;	//	arrived at target
				}
				return FAILURE; //	no velocity component
			}
			return FAILURE; //	no accel component
		}
		return FAILURE;	//	no transform component
	}
};