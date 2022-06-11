class CollisionDetectionSystem : public BaseNode
{
private:
	BaseNode* m_pEntityList;

public:
	CollisionDetectionSystem(BaseNode* pEntityList) :
		m_pEntityList(pEntityList),
		BaseNode("CollisionSystem") {};

	bool isSeperatingAxisCollision(Polygon2D A, Polygon2D B)
	{
		Polygon2D* poly1 = &A;
		Polygon2D* poly2 = &B;

		for (int shape = 0; shape < 2; shape++)
		{
			if (shape == 1)
			{
				poly1 = &B;
				poly2 = &A;
			}

			for (int a = 0; a < poly1->getVerticies().size(); a++)
			{
				int b = (a + 1) % poly1->getVerticies().size();

				// edge normal
				Vector2 axisProj = { -(poly1->getVerticies()[b].Y - poly1->getVerticies()[a].Y), poly1->getVerticies()[b].X - poly1->getVerticies()[a].X };
				float d = sqrtf(axisProj.X * axisProj.X + axisProj.Y * axisProj.Y);
				axisProj = { axisProj.X / d, axisProj.Y / d };

				// Work out min and max 1D points for r1
				float min_r1 = INFINITY, max_r1 = -INFINITY;
				for (int p = 0; p < poly1->getVerticies().size(); p++)
				{
					float q = (poly1->getVerticies()[p].X * axisProj.X + poly1->getVerticies()[p].Y * axisProj.Y);
					min_r1 = min(min_r1, q);
					max_r1 = max(max_r1, q);
				}

				// Work out min and max 1D points for r2
				float min_r2 = INFINITY, max_r2 = -INFINITY;
				for (int p = 0; p < poly2->getVerticies().size(); p++)
				{
					float q = (poly2->getVerticies()[p].X * axisProj.X + poly2->getVerticies()[p].Y * axisProj.Y);
					min_r2 = min(min_r2, q);
					max_r2 = max(max_r2, q);
				}

				if (!(max_r2 >= min_r1 && max_r1 >= min_r2))
					return false;
			}
		}

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