
class TileMapChunkController : public BaseNode, EventListener
{
private:
	int chunkSize;
	Vector2 worldMouse;
	DefaultTileMap* current;
	vector<DefaultTileMap*> Chunks;

	void createChunk(Vector2 worldPos)
	{
		Vector2 worldChunk = worldPos / chunkSize;
		Vector2 worldChunkInt((int)worldChunk.X, (int)worldChunk.Y);
		worldChunkInt.mult(chunkSize);
		int minX = worldChunkInt.X;
		int minY = worldChunkInt.Y;
		int maxX = minX + chunkSize;
		int maxY = minY + chunkSize;
		current = new DefaultTileMap(minX, minY, chunkSize, chunkSize);

		Perlin p;
		float* fNoiseSeed2D = p.newSeed2D(chunkSize, chunkSize, 128);
		float* fPerlinNoise2D = p.PerlinNoise2D(chunkSize, chunkSize, fNoiseSeed2D, 2, 1.0);
		current->initialize(fPerlinNoise2D, chunkSize, chunkSize);
		Chunks.push_back(current);
	}

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
						createChunk(worldMouse);
					}
				}

			} break;
		}
	}

public:
	TileMapChunkController(int dim = 16) :
		chunkSize(dim)
	{
		createChunk({ 0, 0 });
		registerListener(MOUSEWORLD_EVENT);
	};

	~TileMapChunkController()
	{
		for (auto c : Chunks)
		{
			delete c;
			c = NULL;
		}
		Chunks.clear();
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
		int chunksRendered = 0;
		for (auto c : Chunks)
		{
			if (isQuadvQuad(c->getPosition(), c->getPosition() + c->getSize(), vWorldMin, vWorldMax) ||
				isQuadvQuad(vWorldMin, vWorldMax, c->getPosition(), c->getPosition() + c->getSize()))
			{
				c->render(pRenderer, vCameraPosition, vWorldMin, vWorldMax);
				chunksRendered++;
			}
		}
		pRenderer->DrawNum<int>(chunksRendered, 4, 4);
//		renderChunkUnderMouse(pRenderer, vCameraPosition, vWorldMin, vWorldMax);
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