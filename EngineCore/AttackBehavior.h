class AttackBehavior : public LeafNode, EventListener
{
private:

public:
	AttackBehavior() : 
		LeafNode("Attack") {};

	void reset()
	{
		m_nState = IDLE;
	}

	string description()
	{
		switch (m_nState)
		{
			case RUNNING:
				return "Attacking target.";
				break;
			case SUCCESS:
				return "Target destroyed.";
				break;
			case IDLE:
				return "Waiting to attack";
				break;
			case FAILURE:
				return "Target location not reachable.";
				break;
		}
	}

	int execute(float fDeltaTime)
	{
		//	if target is alive
		if (Blackboard->m_pTarget == NULL)
			return FAILURE;
	
		//	if in range
		Vector2 vTarget = Blackboard->m_pTarget->getChild<Transform2D>()->getPosition();
		Vector2 vForward = Blackboard->m_pSelf->getChild<Transform2D>()->getForward();
		Vector2 vPosition = Blackboard->m_pSelf->getChild<Transform2D>()->getPosition();
		float fRange = distance(vPosition, vTarget);

		Vector2 direct = (vPosition - vTarget).normalize();
		float dot = dotProduct(direct, vForward);

		if (fRange >= 35.0)
			return FAILURE;	// to far away

		if (fRange < 35.0)
		{
			if (ShootAction* pShoot = Blackboard->m_pSelf->getChild<ShootAction>())
			{
				if (dot <= 1.0 && dot >= 0.8)
				{
					pShoot->tryAction();
					return RUNNING;

				}
			}
			return FAILURE;	//	no shoot component
		}

		if (fRange < 2.0)
			return FAILURE;	//	too close
	}
};