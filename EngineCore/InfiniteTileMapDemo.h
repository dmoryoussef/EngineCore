
class TileMapChunkController : public BaseNode, EventListener
{
private:
	int chunkSize;
	Vector2 worldMouse;
	DefaultTileMap* current;
	vector<DefaultTileMap*> Chunks;

	DefaultTileMap* getChunk(Vector2 min, Vector2 max)
	{
		for (auto c : Chunks)
		{
			if (isQuadvQuad(min, max, c->getPosition(), c->getPosition() + c->getSize()))
				return c;
		}
		return NULL;
	}

	DefaultTileMap* getChunk(float x, float y)
	{
		for (auto c : Chunks)
		{
			if (isPointvQuad({x, y}, c->getPosition(), c->getPosition() + c->getSize()))
			{
				return c;
			}
		}

		return NULL;
	}

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case MOUSEWORLD_EVENT:
			{
				MouseWorldEvent* pMouseEvent = pEvent->get<MouseWorldEvent>();
				worldMouse = pMouseEvent->getWorldPosition();
				current = getChunk(worldMouse.X, worldMouse.Y);
				if (current == NULL)
				{
					if (pMouseEvent->getState().bLeftButtonDown)
					{
						Vector2 worldChunk = worldMouse / chunkSize;
						Vector2 worldChunkInt((int)worldChunk.X, (int)worldChunk.Y);
						worldChunkInt.mult(chunkSize);
						current = new DefaultTileMap(worldChunkInt.X, worldChunkInt.Y, chunkSize, chunkSize);
						Chunks.push_back(current);
					}
				}

			} break;
		}
	}

public:
	TileMapChunkController() :
		chunkSize(10)
	{
		Chunks.push_back(new DefaultTileMap(10, 10, chunkSize, chunkSize));
		Chunks.push_back(new DefaultTileMap(0, 0, chunkSize, chunkSize));
		Chunks.push_back(new DefaultTileMap(0, 10, chunkSize, chunkSize));
		Chunks.push_back(new DefaultTileMap(10, 0, chunkSize, chunkSize));
		Chunks.push_back(new DefaultTileMap(20, 0, chunkSize, chunkSize));

		registerListener(MOUSEWORLD_EVENT);
	};

	~TileMapChunkController()
	{
		for (auto c : Chunks)
		{
			delete c;
		}
	}

	DefaultTile* tile(int worldX, int worldY)
	{
		for (auto c : Chunks)
		{
			//if (c->getPosition())
		}
		return NULL;
	}

	void renderChunkUnderMouse(Render2D* pRenderer, Vector3 vCameraPosition, Vector2 vWorldMin, Vector2 vWorldMax)
	{
		if (current)
			current->render(pRenderer, vCameraPosition, vWorldMin, vWorldMax);
	}

	void render(Render2D* pRenderer, Vector3 vCameraPosition, Vector2 vWorldMin, Vector2 vWorldMax)
	{
		//	make new list of only chunks currently in the worldview camera quad
	/*	for (auto c : Chunks)
		{
			c->render(pRenderer, vCameraPosition, vWorldMin, vWorldMax);
		}*/

		for (auto c : Chunks)
		{
			if (isQuadvQuad(c->getPosition(), c->getPosition() + c->getSize(), vWorldMin, vWorldMax))
			{
				c->render(pRenderer, vCameraPosition, vWorldMin, vWorldMax);
			}
		}

		renderChunkUnderMouse(pRenderer, vCameraPosition, vWorldMin, vWorldMax);
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