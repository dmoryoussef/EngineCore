class SeekBehaviorNode : public LeafNode
{
private:
	BaseNode* m_pSelf;
	Vector2 vTarget;


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

	}

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
						float turnFactor = 0.1;
						vSteer = (vDesiredDirection - vVelocity).normalize() * turnFactor;
						
						float fThrust = 0.009;
						vAccel = vSteer * fThrust;

						//accel->setForce(vAccel);
						pVelocity->setVelocity(vVelocity + vAccel);
						
						// move to physics?
						pTransform->setRotation({ -vVelocity.X, vVelocity.Y });
					
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

	//void renderNodeData(Render2D* renderer)
	//{
	//	if (m_nState == RUNNING)
	//	{
	//		renderer->DrawVector(vVelocity * 25, vPosition, {PIXEL_SOLID, FG_LIGHTGREEN});
	//		//renderer->DrawString(thingToString<float>(vVelocity.magnitude()), 5, 4);
	//		renderer->DrawVector(vDesiredDirection, vPosition, { PIXEL_SOLID, FG_LIGHTBLUE }); 
	//		//renderer->DrawString(thingToString<float>(vDesiredDirection.magnitude()), 5, 5);
	//		renderer->DrawVector(vSteer, vPosition, {PIXEL_SOLID, FG_LIGHTRED});

	//		renderer->DrawString(thingToString<float>(vAccel.magnitude()), 5, 5);
	//	}
	//}

};