class EntityRenderSystem : public BaseNode
{
public:
	EntityRenderSystem() :
		BaseNode() {}

	void render(BaseNode *pEntities, Render2D *pRenderer, Vector3 vCameraPosition, Vector2 vWorldMin, Vector2 vWorldMax)
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
					float fCameraScale = vCameraPosition.Z;

					Polygon2D polygon = pRender->getPolygon();

					polygon.transform(vScale, vRotation.getAngle(), vPosition);

					polygon.transform({ fCameraScale, fCameraScale }, 0, vCameraPosition.toVec2());

					Pixel color = { PIXEL_SOLID, FG_WHITE };
					if (Collider* pCollider = pEntities->getCurrent()->getChild<Collider>())
						if (pCollider->isColliding())
							color = { PIXEL_SOLID, FG_DARKRED };

					// draw poly
					pRenderer->DrawPoly(polygon.getVerticies(), color);

					// draw forward direction
					Vector2 vTransformedPos = vPosition * fCameraScale;
					vTransformedPos = vTransformedPos + vCameraPosition.toVec2();
					pRenderer->DrawLine(vTransformedPos, polygon.getVerticies()[0], color);
				}
			}

		}
	}

};