class AttackBehavior : public LeafNode
{
private:

public:
	AttackBehavior() {};

	void reset()
	{

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
		//	if in range
		//	else 
		//	return FAILURE
		//	try to attack
		//	return RUNNING;
	}
};