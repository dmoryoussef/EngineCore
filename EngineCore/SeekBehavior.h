class SeekBehavior : public LeafNode
{
private:

public:
	SeekBehavior() :
		LeafNode("Seek") {};

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

	Vector2 vTarget;
	Vector2 vPosition;
	Vector2 vDesiredDirection;
	Vector2 vVelocity;
	Vector2 vSteer;
	Vector2 vAccel;

	int execute(float fDeltaTime)
	{
		if (Blackboard->m_pSelf == NULL) return FAILURE;
		if (Blackboard->m_pTarget == NULL) return FAILURE;

		if (Transform2D* pTransform = Blackboard->m_pSelf->getChild<Transform2D>())
		{
			if (Velocity* pVelocity = Blackboard->m_pSelf->getChild<Velocity>())
			{
				vTarget = Blackboard->m_pTarget->getChild<Transform2D>()->getPosition();
				vPosition = pTransform->getPosition();
				float fDistance = distance(vTarget, vPosition);
				if (fDistance > 35.0)
				{
					//	vector with length total distance to target, normalized
					vDesiredDirection = (vTarget - vPosition).normalize();	
						
					//	scaled up to maximum speed
					/*float maxSpeed = 1.0f;
					vDesiredDirection.mult(maxSpeed);*/

					//	steering force vector, normalized
					vVelocity = pVelocity->getVelocity();
					vSteer = (vDesiredDirection - vVelocity).normalize();

					//	accel force scaled to entity thrust
					setForce(Blackboard->m_pSelf, vSteer);
					pTransform->setRotation(-vSteer);

					//	TO DO:
					//	convert to steering only affects rotation
					//	when rotated to correct vector, accel at max
					//	in proper direction

					return RUNNING;	//	not at target yet
				}
				return SUCCESS;	//	arrived at target
			}
			return FAILURE; //	no velocity component
		}
		return FAILURE;	//	no transform component
	}

	void renderNodeData(Render2D* renderer, Vector2 vMin)
	{
		if (m_nState == RUNNING)
		{
			//renderer->DrawCircle(vPosition.X, vPosition.Y, 15, {PIXEL_SOLID, FG_WHITE});
		}
	}

};