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

			mat3x3 mScale = mScale.Scale(pTransform->getScale());
			mat3x3 mRotate = mRotate.RotateZ(pTransform->getRotation().getAngle());
			mat3x3 mTranslate = mTranslate.Translate(localPos);

			mat3x3 mLocal = mLocal.Identity();
			mLocal = mLocal * mScale;
			mLocal = mLocal * mRotate;
			mLocal = mLocal * mTranslate;

			mWorld = mLocal * mWorld;
		
			if (worldPos.X > vWorldMin.X &&	//	change to a pointvquad collision check for consistancy?  
				worldPos.X < vWorldMax.X &&	//	change to add full bounding quad not just position vector
				worldPos.Y > vWorldMin.Y &&
				worldPos.Y < vWorldMax.Y)
			{
				Polygon2D polygon = pRender->getPolygon();
				vector<Vector2> transVerts = polygon.transformedVerts(mWorld);

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

				//	override with entity collision color
				if (Collider2D* pCollider = pEntity->getChild<Collider2D>())
					if (pCollider->isColliding())
						color = { PIXEL_SOLID, FG_DARKRED };

				// draw poly
				r->DrawPoly(transVerts, color);

				// draw forward direction
				r->DrawLine(worldPos, transVerts[0], color);


				//	if (pEntity->getParent() == NULL)
				{
					vector<string> EntityData;
					EntityData.push_back(pEntity->getName());
					EntityData.push_back(worldPos.toString());
					EntityData.push_back(pTransform->getRotation().toString());

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