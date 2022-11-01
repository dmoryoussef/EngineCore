class SeekBehaviorNode : public LeafNode, EventListener
{
private:
	BaseNode* m_pSelf;
	Vector2 vTarget;

	void onMouseWorldEvent(MouseWorldEvent* pEvent)
	{
		if (m_nState == RUNNING)
		{
			vTarget = pEvent->getWorldPosition();
		}
	}

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case MOUSEWORLD_EVENT: onMouseWorldEvent(pEvent->get<MouseWorldEvent>());
				break;
		}
	}

public:
	SeekBehaviorNode(BaseNode *pSelf) : 
		m_pSelf(pSelf) 
	{
		registerListener(MOUSEWORLD_EVENT);
	};

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
				Vector2 pos = pTransform->getPosition();
				float fDistance = distance(vTarget, pos);
				if (fDistance > 1.0)
				{
					float speed = 0.005;
					Vector2 vDesiredDirection = (vTarget - pos).normalize();
					Vector2 vAccel = vDesiredDirection * speed;
					if (Velocity* pVelocity = m_pSelf->getChild<Velocity>())
					{
						Vector2 vVelocity = pVelocity->getVelocity();
						Vector2 vSteer = vDesiredDirection - vAccel;
						float max = 0.0001;
						vSteer = vSteer.limit(max);

						//	mult by deltatime in physics or here?
						accel->setForce(vSteer * fDeltaTime);
						

						// move to physics?
						pTransform->setRotation({ -vVelocity.X, vVelocity.Y });
					}
					return RUNNING;	//	not at target yet
				}
				return SUCCESS;	//	arrived at target
			}
			return FAILURE; //	no accel component
		}
		return FAILURE;	//	no transform component
	}

};