class TileMapChunkController : public BaseNode
{
private:
	vector<DefaultTileMap*> Chunks;

public:
	TileMapChunkController()
	{
		Chunks.push_back(new DefaultTileMap(10, 10, 10, 10));
		Chunks.push_back(new DefaultTileMap(0, 0, 10, 10));
		Chunks.push_back(new DefaultTileMap(0, 10, 10, 10));
		Chunks.push_back(new DefaultTileMap(10, 0, 10, 10));
		Chunks.push_back(new DefaultTileMap(20, 0, 10, 10));
	};

	DefaultTile* tile(int worldX, int worldY)
	{
		for (auto c : Chunks)
		{
			//if (c->getPosition())
		}
		return NULL;
	}

	void render(Render2D* pRenderer, Vector3 vCameraPosition, Vector2 vWorldMin, Vector2 vWorldMax)
	{
		//	make new list of only chunks currently in the worldview camera quad
		for (auto c : Chunks)
		{
			c->render(pRenderer, vCameraPosition, vWorldMin, vWorldMax);
		}
	}
};

class InfiniteTileMapDemo : public GameState
{
private:

public:
	InfiniteTileMapDemo() {};

	void start(BaseNode* pData, BaseNode* pSystems, BaseNode* pGUI)
	{
		GameState::start(pData, pSystems, pGUI);

		pData->add(new TileMapChunkController());
	}

	void update(BaseNode* pData, float fDeltaTime) {}

	void render(OutputBuffer* pEngineBuffer)
	{
		Render2D r(pEngineBuffer, camera->getChild<Transform3D>()->getPosition());
	}
};