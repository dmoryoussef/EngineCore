class SeekBehaviorNode : public LeafNode
{
private:
	BaseNode* m_pSelf;

public:
	SeekBehaviorNode(BaseNode *pSelf) : 
		m_pSelf(pSelf) {};

	string description()
	{
		switch (m_nState)
		{
			case RUNNING:
				return "Seeking target.";
				break;
			case SUCCESS:
				return "Arrived at target.";
				break;
			case IDLE:
				return "Waiting to move";
				break;
			case FAILURE:
				return "Target location not reachable.";
				break;
		}
	}

	void reset()
	{
		m_nState = IDLE;
	}

	Vector2 vTarget;
	Vector2 vPosition;
	Vector2 vDesiredDirection;
	Vector2 vVelocity;
	Vector2 vSteer;
	Vector2 vAccel;

	int execute(float fDeltaTime)
	{
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
					if (fDistance > 1.0)
					{
						vDesiredDirection = (vTarget - vPosition).normalize();
					
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

	void renderNodeData(Render2D* renderer)
	{
		if (m_nState == RUNNING)
		{
			//renderer->DrawCircle(vPosition.X, vPosition.Y, 15, {PIXEL_SOLID, FG_WHITE});
		}
	}

};