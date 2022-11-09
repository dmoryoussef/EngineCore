class FleeBehavior : public LeafNode
{
private:

public:
	FleeBehavior(){};

	int execute(float fDeltaTime)
	{
		Vector2 vTarget;
		Vector2 vPosition;
		Vector2 vDesiredDirection;
		Vector2 vVelocity;
		Vector2 vSteer;
		Vector2 vAccel;

		if (Blackboard->m_pSelf == NULL)
		{
			return FAILURE;
		}

		if (Transform2D* pTransform = Blackboard->m_pSelf->getChild<Transform2D>())
		{
			if (Accelerate* accel = Blackboard->m_pSelf->getChild<Accelerate>())
			{
				if (Velocity* pVelocity = Blackboard->m_pSelf->getChild<Velocity>())
				{
					if (Transform2D* pTargetTransform = Blackboard->m_pTarget->getChild<Transform2D>())
					{
						vTarget = pTargetTransform->getPosition();
						vPosition = pTransform->getPosition();
						float fDistance = distance(vTarget, vPosition);
						if (fDistance < 25.0)
						{
							vDesiredDirection = -(vTarget - vPosition).normalize();
							//	needs a randomizing agent for "juking"?

							vVelocity = pVelocity->getVelocity();
							vSteer = (vDesiredDirection - vVelocity).normalize();

							float fThrust = accel->getMax();
							vAccel = vSteer * fThrust;

							accel->setForce(vAccel);

							return RUNNING;	//	not at target yet
						}
						return SUCCESS;	//	arrived at target
					}
					return FAILURE; //	target has no transform component
				}
				return FAILURE; //	no velocity component
			}
			return FAILURE; //	no accel component
		}
		return FAILURE;	//	no transform component
	}
};