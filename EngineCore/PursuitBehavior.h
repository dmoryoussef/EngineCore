class PursuitBehaviorNode : public LeafNode
{
private:
	Vector2 vTargetPrev;
	Vector2 vPursueVelocity;

public:
	PursuitBehaviorNode() :
		vTargetPrev() {}

	int execute(float fDeltaTime)
	{
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
					Vector2 vPosition = pTransform->getPosition();
					Vector2 vTarget = Blackboard->m_pTarget->getChild<Transform2D>()->getPosition();

					vPursueVelocity = (vTarget - vTargetPrev) * (fDeltaTime * 5);
					Vector2 vNewTargetPosition = vTarget + vPursueVelocity;
					
					vTargetPrev = vTarget;

					float fDistance = distance(vTarget, vPosition);
					if (fDistance > 1.0)
					{
						//	vector with length total distance to target, normalized
						Vector2 vDesiredDirection = (vNewTargetPosition - vPosition).normalize();

						//	scaled up to maximum speed
						float maxSpeed = 1.0f;
						vDesiredDirection.mult(maxSpeed);

						//	steering force vector, normalized
						Vector2 vVelocity = pVelocity->getVelocity();
						Vector2 vSteer = (vDesiredDirection - vVelocity).normalize();

						//	accel force scaled to entity thrust
						float fThrust = accel->getMax();
						Vector2 vAccel = vSteer * fThrust;

						accel->setForce(vAccel);

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
			return FAILURE; //	no accel component
		}
		return FAILURE;	//	no transform component
	}

	void renderNodeData(Render2D* renderer)
	{
		// renderer->DrawCircle(Blackboard->vTarget.X, Blackboard->vTarget.Y, 1.0, {PIXEL_SOLID, FG_WHITE});
	}
};