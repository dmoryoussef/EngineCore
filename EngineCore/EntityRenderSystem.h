class EntityRenderSystem : public BaseNode
{
public:
	EntityRenderSystem() :
		BaseNode() {}

	void render(BaseNode* pEntities, Render2D* pRenderer, Vector2 vWorldMin, Vector2 vWorldMax)
	{
		while (pEntities->isIterating())
		{
			Transform2D* pTransform = pEntities->getCurrent()->getChild<Transform2D>();
			Render* pRender = pEntities->getCurrent()->getChild<Render>();


			if (pTransform && pRender)
			{
				Vector2 vPosition = pTransform->getPosition();
				Vector2 vScale = pTransform->getScale();
				Vector2 vRotation = pTransform->getRotation();

				if (vPosition.X > vWorldMin.X &&
					vPosition.X < vWorldMax.X &&
					vPosition.Y > vWorldMin.Y &&
					vPosition.Y < vWorldMax.Y)
				{

					Polygon2D polygon = pRender->getPolygon();

					polygon.transform(vScale, vRotation.getAngle(), vPosition);

					//polygon.transform({ fCameraScale, fCameraScale }, 0, vCameraPosition.toVec2());

					Pixel color = { PIXEL_SOLID, FG_WHITE };

					if (UIState* pUIState = pEntities->getCurrent()->getChild<UIState>())
					{
						switch (pUIState->getState())
						{
							case MOUSE_OVER: color = { PIXEL_SOLID, FG_LIGHTBLUE };
										   break;

							case LEFT_PRESSED: color = { PIXEL_SOLID, FG_DARKBLUE };
											 break;

							case LEFT_RELEASED: color = { PIXEL_SOLID, FG_LIGHTBLUE };
											  break;
						}
					}

					//	override with entity collision color
					if (Collider2D* pCollider = pEntities->getCurrent()->getChild<Collider2D>())
						if (pCollider->isColliding())
							color = { PIXEL_SOLID, FG_DARKRED };

					// draw poly
					pRenderer->DrawPoly(polygon.getVerticies(), color);



					// draw forward direction
					pRenderer->DrawLine(vPosition, polygon.getVerticies()[0], color);

					vector<string> EntityData;

					if (Transform2D* pTransform = pEntities->getCurrent()->getChild<Transform2D>())
						EntityData.push_back(pTransform->getPosition().toString());

					if (Health* pHealth = pEntities->getCurrent()->getChild<Health>())
						EntityData.push_back(thingToString(pHealth->getHealth()));

					Vector2 pos(1.5, -0.5);
					pos = pos + vPosition;
					pRenderer->DrawString(EntityData, pos.X, pos.Y);

				}
			}

		}
	}

};