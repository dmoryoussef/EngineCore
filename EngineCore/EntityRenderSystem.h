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
					polygon.scale(vScale);
					polygon.rotate(vRotation.getAngle());
					polygon.translate(vPosition);
					
					polygon.scale({ fCameraScale, fCameraScale });
					polygon.translate(vCameraPosition.toVec2());

					pRenderer->DrawPoly(polygon.getVerticies(), Pixel(PIXEL_SOLID, FG_WHITE));
				}
			}

		}
	}

};