class CollisionDetectionSystem : public BaseNode, EventListener
{
private:
	BaseNode* m_pEntityList;


	float seperatingAxisCollisionVectorsOverlap(vector<Vector2> polyA, vector<Vector2> polyB)
	{
		float overlap = INFINITY;

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
			float min_r2 = INFINITY;
			float max_r2 = -INFINITY;
			for (int p = 0; p < polyB.size(); p++)
			{
				float dp = dotProduct(polyB[p], axisProj);
				min_r2 = min(min_r2, dp);
				max_r2 = max(max_r2, dp);
			}

			overlap = min(min(max_r1, max_r2) - max(min_r1, min_r2), overlap);
		}
			
		return overlap;
	}

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
			float min_r2 = INFINITY;
			float max_r2 = -INFINITY;
			for (int p = 0; p < polyB.size(); p++)
			{
				float dp = dotProduct(polyB[p], axisProj);
				min_r2 = min(min_r2, dp);
				max_r2 = max(max_r2, dp);
			}

			if (!(max_r2 >= min_r1 && max_r1 >= min_r2))
				return false;
		}

		return true;
	}

	bool isSeperatingAxisCollision(vector<Vector2> polyA, vector<Vector2> polyB)
	{
		if (!isSeperatingAxisCollisionVectors(polyA, polyB))
			return false;

		if (!isSeperatingAxisCollisionVectors(polyB, polyA))
			return false;

		return true;
	}

	void updateState(BaseNode *pEntity, mat3x3 mWorld, vector<Vector2> mousePosition, MouseState mouseState)
	{
		if (UIState* pUIState = pEntity->getChild<UIState>())
			if (Render* pRender = pEntity->getChild<Render>())
				if (Transform2D* pTransform = pEntity->getChild<Transform2D>())
				{
					Polygon2D poly = pRender->getPolygon();
					Polygon2D transformedPoly = poly;

					mat3x3 mLocal = pTransform->getTransformMatrix();
					mWorld = mLocal * mWorld;

					vector<Vector2> transVerts = poly.transformedVerts(mWorld);

					bool bMouseOver = (isSeperatingAxisCollisionVectors(transVerts, mousePosition));

					pUIState->setState(mouseState.updateState(bMouseOver, pUIState->isActive(), pUIState->getState()));
				}

		while (pEntity->isIterating())
		{
			BaseNode* pCurrent = pEntity->getCurrent();
			updateState(pCurrent, mWorld, mousePosition, mouseState);
		}
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
				mat3x3 mWorld = mWorld.Identity();
				while (m_pEntityList->isIterating())
				{
					BaseNode* pEntity = m_pEntityList->getCurrent();
					updateState(pEntity, mWorld, mousePosition, mouseState);
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
			if (Collider2D* pCollider = m_pEntityList->getCurrent()->getChild<Collider2D>())
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
			if (Collider2D *pColliderA = pEntityA->getChild<Collider2D>())
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
						if (Collider2D* pColliderB = pEntityB->getChild<Collider2D>())
							if (Transform2D* pTransformB = pEntityB->getChild<Transform2D>())
							{
								mat3x3 mLocalA = pTransformA->getTransformMatrix();
								vector<Vector2> transVertsA = pColliderA->getPoly().transformedVerts(mLocalA);
								mat3x3 mLocalB = pTransformB->getTransformMatrix();
								vector<Vector2> transVertsB = pColliderB->getPoly().transformedVerts(mLocalB);

								if (isSeperatingAxisCollision(transVertsA, transVertsB))
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