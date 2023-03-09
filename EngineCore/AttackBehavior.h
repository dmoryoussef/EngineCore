class AttackBehavior : public LeafNode, EventListener
{
private:
	BaseNode* m_pSelf;

public:
	AttackBehavior(BaseNode *pSelf) : 
		m_pSelf(pSelf),
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
		//if (Blackboard->pTarget)
		{
			//	if in range
			//	Vector2 vTarget = Blackboard->pTarget->getChild<Transform2D>()->getPosition();
			Vector2 vTarget = Blackboard->vTarget;
			Vector2 vPosition = m_pSelf->getChild<Transform2D>()->getPosition();
			float fRange = distance(vPosition, vTarget);
			if (fRange > 15.0)
				return FAILURE;	// to far away

			if (fRange < 15.0)
			{
				if (ShootAction* pShoot = m_pSelf->getChild<ShootAction>())
				{
					if (pShoot->canShoot())
					{
						addEvent(new CommandEvent(m_pSelf, new ActionCommand()));
					}
					return RUNNING;
				}
				return FAILURE;	//	no shoot component
			}

			if (fRange < 2.0)
				return FAILURE;	//	too close
		}
		//return FAILURE;	//	no target
	}
};