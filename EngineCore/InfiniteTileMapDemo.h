

class TileMapChunkController : public BaseNode, EventListener
{
private:
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
				current = getChunk(pMouseEvent->getWorldPosition().X, pMouseEvent->getWorldPosition().Y);

			} break;
		}
	}

public:
	TileMapChunkController()
	{
		Chunks.push_back(new DefaultTileMap(10, 10, 10, 10));
		Chunks.push_back(new DefaultTileMap(0, 0, 10, 10));
		Chunks.push_back(new DefaultTileMap(0, 10, 10, 10));
		Chunks.push_back(new DefaultTileMap(10, 0, 10, 10));
		Chunks.push_back(new DefaultTileMap(20, 0, 10, 10));

		registerListener(MOUSEWORLD_EVENT);
	};

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