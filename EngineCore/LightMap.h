class LightMap : public _TileMap<LightTile>
{
private:
	void onMouseWorldEvent(MouseWorldEvent* pEvent)
	{
		if (pEvent->getState().bLeftButtonDown)
		{
			Vector2 pos = pEvent->getWorldPosition();
			if (LightTile* pTile = getWorldTile(pos.X, pos.Y))
			{
				pTile->setBlocking(true);
			}
		}
	}

	void onEvent(_Event* pEvent)
	{
		_TileMap::onEvent(pEvent);

		switch (pEvent->m_eType)
		{
		case MOUSEWORLD_EVENT: onMouseWorldEvent(pEvent->get<MouseWorldEvent>());
			break;
		}
	}
public:
	LightMap(int width, int height) :
		_TileMap({ (float)width, (float)height }, "LIGHTMAP")
	{
		Position.X = 5;
		Position.Y = 5;
		registerListener(MOUSEWORLD_EVENT);
	}

	bool isLineOfSight(Vector2 A, Vector2 B)
	{
		if (B == doRayCast(A, B))
		{
			return true;
		}
		return false;
	}

	void applyLight(Vector2 position, float brightness)
	{
		//float radius = brightness * 15; // figure out a better way to calculate this value!

		Vector2 lightPos = position - Position;
		/*for (int y = position.Y - radius / 2; y < position.Y + radius / 2; y++)
		{
			for (int x = position.X - radius / 2; x < position.X + radius / 2; x++)
			{*/
		for (int y = Position.Y; y < Size.Y + Position.Y; y++)
		{
			for (int x = Position.X; x < Size.X + Position.Y; x++)
			{

				if (LightTile* pTile = getWorldTile(x, y))
				{
					Vector2 tilePos = pTile->getLocalPosition();
					if (isLineOfSight(lightPos, tilePos))
					{
						float lightDistance = distance(tilePos, lightPos);
						float fAttenuation1 = 1;	// research the actual terms!
						float fAttenuation2 = 1;
						float fAttenuation3 = .05;
						float fAttentuationFactor = fAttenuation1 + (fAttenuation2 * lightDistance) + (fAttenuation3 * (lightDistance * lightDistance));
						float fFinalBrightness = brightness / fAttentuationFactor;

						fFinalBrightness = clamp(fFinalBrightness, 0.01, 1);

						pTile->setValue(fFinalBrightness);
					}
					else
					{
						pTile->setValue(0);
					}
				}
			}
		}
	}



	void render(Render2D* pRenderer, Vector3 vCameraPosition, Vector2 vWorldMin, Vector2 vWorldMax)
	{
		_TileMap::render(pRenderer, vCameraPosition, vWorldMin, vWorldMax);

		//	render clipped part of map
		Vector2 vTileMin = clipMin(vWorldMin);
		Vector2 vTileMax = clipMax(vWorldMax);
		float fTileSize = 1.0f;

		//	draw any blocking tiles
		for (int nY = vTileMin.Y; nY < vTileMax.Y; nY++)
			for (int nX = vTileMin.X; nX < vTileMax.X; nX++)
			{

				Vector2 Min = Vector2(nX, nY) + Position;
				Vector2 Max = Min + Vector2(fTileSize, fTileSize);

				if (getTile(nX, nY)->isBlocking())
					pRenderer->DrawQuad(Min.X,
						Min.Y,
						Max.X,
						Max.Y,
						Pixel(PIXEL_SOLID, FG_WHITE));
			}
	}
};