class EntityRenderSystem : public BaseNode
{
public:
	EntityRenderSystem() :
		BaseNode() {}

	void render(BaseNode *pEntities, Render2D *pRenderer, Vector3 vCameraPosition, Vector2 vWorldMin, Vector2 vWorldMax)
	{
		int renderedEntities = 0;
		while (pEntities->isIterating())
		{

			Transform* pTransform = pEntities->getCurrent()->getChild<Transform>();
			Render* pRender = pEntities->getCurrent()->getChild<Render>();

			if (pTransform && pRender)
			{
				Vector3 vPosition = pTransform->getPosition();
				
				//if (vPosition.X > vWorldMin.X && 
				//	vPosition.X < vWorldMax.X && 
				//	vPosition.Y > vWorldMin.Y && 
				//	vPosition.Y < vWorldMax.Y)
				{
					float fScale = vCameraPosition.Z;
					mat3x3 trans = trans.Translate(vPosition.toVec2());
					mat3x3 camTrans = trans.Translate({ -vCameraPosition.toVec2().X, -vCameraPosition.toVec2().Y });
					mat3x3 rotate = rotate.Rotate(3);
					mat3x3 scale = scale.Scale({ fScale, fScale });

					Triangle2D tri({ -0.5, 0 }, {0, 1 }, { 0.5, 0 });
					
					tri = tri * rotate;
					tri = tri * scale;
					tri = tri * trans;
					
					//tri = tri * camTrans;
					//	rotate

					pRenderer->DrawTriangle(tri.A, tri.B, tri.C, Pixel(PIXEL_SOLID, FG_WHITE));
					renderedEntities++;
				}
					
				
			}

		}
		pRenderer->DrawNum(renderedEntities, 2, 2, FG_WHITE);
	}

};