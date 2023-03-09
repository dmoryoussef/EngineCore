class EntityRenderSystem : public BaseNode
{
public:
	EntityRenderSystem() :
		BaseNode() {}

	void renderEntity(Render2D *r, BaseNode *pEntity, mat3x3 mWorld, Vector2 vWorldMin, Vector2 vWorldMax)
	{
		Transform2D* pTransform = pEntity->getChild<Transform2D>();
		Render* pRender = pEntity->getChild<Render>();

		if (pTransform && pRender)
		{
			Vector2 localPos = pTransform->getPosition();
			Vector2 worldPos = localPos * mWorld;

			mat3x3 mLocal = pTransform->getTransformMatrix();
			mWorld = mLocal * mWorld;
		
			Polygon2D polygon = pRender->getPolygon();
			vector<Vector2> transVerts = polygon.transformedVerts(mWorld);
			
			if (isPolyvQuad(transVerts, vWorldMin, vWorldMax)) // only render if at least part of the poly is on screen
			{
				Pixel color = { PIXEL_SOLID, FG_WHITE };

				if (UIState* pUIState = pEntity->getChild<UIState>())
				{
					switch (pUIState->getState())
					{
						case MOUSE_OVER: color = { PIXEL_SOLID, FG_LIGHTBLUE };	break;
						case LEFT_PRESSED: color = { PIXEL_SOLID, FG_DARKBLUE }; break;
						case LEFT_RELEASED: color = { PIXEL_SOLID, FG_LIGHTBLUE }; break;
					}
				}

				//	draw collider box and state
				if (Collider2D* pCollider = pEntity->getChild<Collider2D>())
				{
					Pixel colliderColor = { PIXEL_QUARTER, FG_DARKRED };
					if (pCollider->isColliding())
						colliderColor = { PIXEL_SOLID, FG_DARKRED };
					vector<Vector2> colliderVerts = pCollider->getPoly().transformedVerts(mWorld);
					r->DrawPoly(colliderVerts, colliderColor);
				}

				// draw poly
				r->DrawPoly(transVerts, color);

				// draw forward direction
				r->DrawLine(worldPos, transVerts[0], color);

				if (DetailsPanel *pPanel = pEntity->getChild<DetailsPanel>())	//	find a better way to detect root node
				{																//	or a better way for info panel gui
					vector<string> EntityData;
					EntityData.push_back(pEntity->getName());
					EntityData.push_back(worldPos.toString());
					//EntityData.push_back("Rotation: " + thingToString<float>(pTransform->getRotation().getAngle()));
					//EntityData.push_back("Forward: " + thingToString<float>(pTransform->getForward().getAngle()));
					/*if (Velocity* pVelocity = pEntity->getChild<Velocity>())
					{
						EntityData.push_back(thingToString<float>(pVelocity->getVelocity().magnitude()));
					}*/
					if (Health* pHealth = pEntity->getChild<Health>())
						EntityData.push_back(thingToString(pHealth->getHealth()));

					Vector2 pos(1.5, -0.5);
					pos = pos + worldPos;
					r->DrawString(EntityData, pos.X, pos.Y);
				}
			}
		}

		while (pEntity->isIterating())
		{
			BaseNode* pCurrent = pEntity->getCurrent();
			renderEntity(r, pCurrent, mWorld, vWorldMin, vWorldMax);
		}
	}

	void render(BaseNode* pEntities, Render2D* pRenderer, Vector2 vWorldMin, Vector2 vWorldMax)
	{
		mat3x3 mWorld = mWorld.Identity();

		while (pEntities->isIterating())
		{
			BaseNode* pCurrent = pEntities->getCurrent();
			renderEntity(pRenderer, pCurrent, mWorld, vWorldMin, vWorldMax);
		}
	}

};