class CollisionDetectionSystem : public BaseNode, EventListener
{
private:
	BaseNode* m_pEntityList;

	bool isSeperatingAxisCollisionVectors(vector<Vector2> polyA, vector<Vector2> polyB)
	{
		for (int a = 0; a < polyA.size(); a++)
		{
			int b = (a + 1) % polyA.size();

			// edge normal
			Vector2 axisProj = getNormal(polyA[a], polyA[b]).normalize();

			// Work out min and max 1D points for r1
			float min_r1 = INFINITY;
			float max_r1 = -INFINITY;
			for (int p = 0; p < polyA.size(); p++)
			{
				float dp = dotProduct(polyA[p], axisProj);
				min_r1 = min(min_r1, dp);
				max_r1 = max(max_r1, dp);
			}

			// Work out min and max 1D points for r2
			float min_r2 = INFINITY, max_r2 = -INFINITY;
			for (int p = 0; p < polyB.size(); p++)
			{
				float dp = dotProduct(polyB[p], axisProj);
				min_r2 = min(min_r2, dp);
				max_r2 = max(max_r2, dp);
			}

			if (!(max_r2 >= min_r1 && max_r1 >= min_r2))
				return false;
		}
	}

	bool isSeperatingAxisCollision(Polygon2D A, Polygon2D B)
	{

		vector<Vector2> polyA = A.getVerticies();
		vector<Vector2> polyB = B.getVerticies();

		if (!isSeperatingAxisCollisionVectors(polyA, polyB))
			return false;

		if (!isSeperatingAxisCollisionVectors(polyB, polyA))
			return false;



		return true;
	}


	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case MOUSEWORLD_EVENT:
			{
				Vector2 vMousePosition = pEvent->get<MouseWorldEvent>()->getWorldPosition();
				MouseState mouseState = pEvent->get<MouseWorldEvent>()->getState();
				vector<Vector2> mousePosition;
				mousePosition.push_back(vMousePosition);

				while (m_pEntityList->isIterating())
				{
					BaseNode* pEntityA = m_pEntityList->getCurrent();
					if (UIState *pUIState = pEntityA->getChild<UIState>())
						if (Collider* pColliderA = pEntityA->getChild<Collider>())
							if (Render* pRenderA = pEntityA->getChild<Render>())
								if (Transform2D* pTransformA = pEntityA->getChild<Transform2D>())
								{
									Polygon2D polyA = pRenderA->getPolygon();
									Polygon2D transformedPolyA = polyA;
									transformedPolyA.transform(pTransformA->getScale(), pTransformA->getRotation().getAngle(), pTransformA->getPosition());

									bool bMouseOver = (isSeperatingAxisCollisionVectors(transformedPolyA.getVerticies(), mousePosition));
										
									pUIState->setState(mouseState.updateState(bMouseOver, pUIState->isActive(), pUIState->getState()));
								}
				}
							
				
				break;
			}
		}
	}

public:
	CollisionDetectionSystem(BaseNode* pEntityList) :
		m_pEntityList(pEntityList),
		BaseNode("CollisionSystem") 
	{
		registerListener(MOUSEWORLD_EVENT);
	};



	void resetColliders()
	{
		while (m_pEntityList->isIterating())
		{
			if (Collider* pCollider = m_pEntityList->getCurrent()->getChild<Collider>())
			{
				pCollider->setColliding(false);
			}
		}
	}

	void handleCollisions()
	{
		//	pass list of only entities with a velocity, only test those that moved
		//	will make a shorter list?
		//	right now, checks everything

		while (m_pEntityList->isIterating())
		{
			BaseNode* pEntityA = m_pEntityList->getCurrent();
			if (Collider *pColliderA = pEntityA->getChild<Collider>())
				if (Render* pRenderA = pEntityA->getChild<Render>())
					if (Transform2D* pTransformA = pEntityA->getChild<Transform2D>())
					{
						//	check against world min/max
						Vector2 vPosition = pTransformA->getPosition();
						Vector2 vWorldMax(75, 75);
						Vector2 vWorldMin(-75, -75);
						if (vPosition.X > vWorldMax.X || vPosition.X < vWorldMin.X ||
							vPosition.Y > vWorldMax.Y || vPosition.Y < vWorldMin.Y)
						{
							pColliderA->setColliding(true);
							if (CollisionResponse* pResponse = pEntityA->getChild<OutOfBoundsCollision>())
							{
								pResponse->onCollision();
							}
						}

						//	check entity collisions
						BaseNode* pEntityB = pEntityA->getNext();
						while (pEntityB)
						{
							if (Collider* pColliderB = pEntityB->getChild<Collider>())
								if (Render* pRenderB = pEntityB->getChild<Render>())
									if (Transform2D* pTransformB = pEntityB->getChild<Transform2D>())
									{
										//	transform polygons
										Polygon2D transformedPolyA = pRenderA->getPolygon();
										Polygon2D transformedPolyB = pRenderB->getPolygon();

										transformedPolyA.transform(pTransformA->getScale(), pTransformA->getRotation().getAngle(), pTransformA->getPosition());
										transformedPolyB.transform(pTransformB->getScale(), pTransformB->getRotation().getAngle(), pTransformB->getPosition());

										if (isSeperatingAxisCollision(transformedPolyA, transformedPolyB))
										{
											pColliderA->setColliding(true);
											pColliderB->setColliding(true);

											// send collision event?
											//	colliding entities
											//	vectors/forces for resolution
											
											//	move to CollisionResponseSystem?????
											//	collision response:

											//	doesnt work correctly probably cant use inheritance for this
											if (CollisionResponse* pResponseA = pEntityA->getChild<OutOfBoundsCollision>())
											{
												pResponseA->onCollision();
											}
											if (CollisionResponse* pResponseB = pEntityB->getChild<OutOfBoundsCollision>())
											{
												pResponseB->onCollision();
											}

											Health* pHealthA = pEntityA->getChild<Health>();
											Damage* pDamageA = pEntityA->getChild<Damage>();

											Health* pHealthB = pEntityB->getChild<Health>();
											Damage* pDamageB = pEntityB->getChild<Damage>();

											if (pHealthA && pDamageB)
											{
												int nCurrentHealth = pHealthA->getHealth();
												int nDamage = pDamageB->getDamage();
												nCurrentHealth = nCurrentHealth - nDamage;
												if (nCurrentHealth < 0)
												{
													nCurrentHealth = 0;
													addEvent(new DeleteBaseNodeEvent(pHealthA->getParent()));
												}
												else
													pHealthA->setHealth(nCurrentHealth);

											}

											if (pHealthB && pDamageA)
											{
												int nCurrentHealth = pHealthB->getHealth();
												int nDamage = pDamageA->getDamage();
												nCurrentHealth = nCurrentHealth - nDamage;
												if (nCurrentHealth < 0)
												{
													nCurrentHealth = 0;
													addEvent(new DeleteBaseNodeEvent(pHealthB->getParent()));
												}
												else
													pHealthB->setHealth(nCurrentHealth);
											}

										}
									}

							pEntityB = pEntityB->getNext();
						}
					}
					
		}
	}

	void update(float fDeltaTime)
	{
		resetColliders();
		handleCollisions();
	}

};