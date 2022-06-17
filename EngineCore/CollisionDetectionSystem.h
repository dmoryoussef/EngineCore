class CollisionDetectionSystem : public BaseNode, EventListener
{
private:
	BaseNode* m_pEntityList;

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case MOUSEWORLD_EVENT:
			{
				Vector2 vMousePosition = pEvent->get<MouseWorldEvent>()->getWorldPosition();
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

									pUIState->setState(DEFAULT);

									if (isSeperatingAxisCollisionVectors(transformedPolyA.getVerticies(), mousePosition))
										pUIState->setState(MOUSE_OVER);

									if (isSeperatingAxisCollisionVectors(mousePosition, transformedPolyA.getVerticies()))
										pUIState->setState(MOUSE_OVER);
								
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

	bool isSeperatingAxisCollisionVectors(vector<Vector2> polyA, vector<Vector2> polyB)
	{
		for (int a = 0; a < polyA.size(); a++)
			{
				int b = (a + 1) % polyA.size();

				// edge normal
				Vector2 axisProj = { -(polyA[b].Y - polyA[a].Y), polyA[b].X - polyA[a].X };
				float d = sqrtf(axisProj.X * axisProj.X + axisProj.Y * axisProj.Y);
				axisProj = { axisProj.X / d, axisProj.Y / d };

				// Work out min and max 1D points for r1
				float min_r1 = INFINITY, max_r1 = -INFINITY;
				for (int p = 0; p < polyA.size(); p++)
				{
					float q = (polyA[p].X * axisProj.X + polyA[p].Y * axisProj.Y);
					min_r1 = min(min_r1, q);
					max_r1 = max(max_r1, q);
				}

				// Work out min and max 1D points for r2
				float min_r2 = INFINITY, max_r2 = -INFINITY;
				for (int p = 0; p < polyB.size(); p++)
				{
					float q = (polyB[p].X * axisProj.X + polyB[p].Y * axisProj.Y);
					min_r2 = min(min_r2, q);
					max_r2 = max(max_r2, q);
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
					if (Transform2D *pTransformA = pEntityA->getChild<Transform2D>())
					{
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