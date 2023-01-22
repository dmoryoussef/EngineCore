
class InfiniteTileMapTile : public _Tile2D<InfiniteTileMapTile>
{
private:
	BaseNode* tesselatedTileMap; 

public:
	InfiniteTileMapTile() :
		tesselatedTileMap(NULL),
		_Tile2D("") {};

	BaseNode* getMap()
	{
		return tesselatedTileMap;
	}

	void setMap(BaseNode *pMap)
	{
		tesselatedTileMap = pMap;
	}
};

class InfiniteTileMap : public _TileMap<InfiniteTileMapTile>
{
private:
	float fTileSize;
	float fZoomLevel;

public:
	InfiniteTileMap(float tileSize, Vector2 position, float zoom) :
		fTileSize(tileSize),
		fZoomLevel(zoom),
		_TileMap({ 10, 10 }, "")
	{
		setPosition(position.X, position.Y);
		createCheckerMap();
	};

	void render(Render2D* pRenderer, Vector3 vCameraPosition, Vector2 vWorldMin, Vector2 vWorldMax)
	{
		// normal
		if (vCameraPosition.Z < fZoomLevel)
			_TileMap::render(pRenderer, vCameraPosition, vWorldMin, vWorldMax);
		else
		{
			//	render based on scale either current tile or submap
			Vector2 vTileMin = clipMin(vWorldMin);
			Vector2 vTileMax = clipMax(vWorldMax);
			float fTileSize = 1.0f;

			//	draw any blocking tiles
			for (int y = vTileMin.Y; y < vTileMax.Y; y++)
				for (int x = vTileMin.X; x < vTileMax.X; x++)
				{

					Vector2 Min = Vector2(x, y) + Position;
					Vector2 Max = Min + Vector2(fTileSize, fTileSize);

					InfiniteTileMapTile* pTile = getTile(x, y);
					if (pTile->getMap() == NULL)
					{
						//	create map
					//	pTile->setMap(new InfiniteTileMap((1/vCameraPosition.Z), pTile->getPosition()));
					}
					
					InfiniteTileMap* map = dynamic_cast<InfiniteTileMap*>(pTile->getMap());
					map->render(pRenderer, vCameraPosition, vWorldMin, vWorldMax);

				}
		}
	}


};

class InfiniteTileMapDemo : public GameState
{
private:
	Transform3D *camera;

public:
	InfiniteTileMapDemo() {};

	void start(BaseNode* pData, BaseNode* pSystems, BaseNode* pGUI)
	{
		int height = pGUI->cast<_UIComponent>()->getHeight();
		int width = pGUI->cast<_UIComponent>()->getWidth();
		CameraViewWindow* pCameraWindow = new CameraViewWindow(width, height, 0, 0);
		BaseNode* pCamera = pCameraWindow->getCamera();
		camera = pCamera->getChild<Transform3D>();
		pGUI->addAtEnd(pCameraWindow);


		pData->add(new InfiniteTileMap(1, {0, 0}, 30));
	}

	void update(BaseNode* pData, float fDeltaTime) {}

	void render(OutputBuffer* pEngineBuffer)
	{
		Render2D r(pEngineBuffer, camera->getPosition());
	}
};