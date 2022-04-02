enum CardinalDirections
{
	EAST,
	WEST,
	NORTH,
	SOUTH
};

template <typename TileType> class _TileMap :
	public BaseNode,
	public EventListener
{
protected:
	Vector2 Position;
	Vector2 Size;
	TileType* m_pTileMap;
	int m_nTotalGroups;

	TileType* m_pMouseOverTile;

	void setBoarders()
	{
		for (int nY = 0; nY < Size.Y; nY++)
			for (int nX = 0; nX < Size.X; nX++)
				if (nY == 0 || nY == Size.Y - 1 || nX == 0 || nX == Size.X - 1)
					getTile(nX, nY)->setValue(1);
	}

	void initialize(float fInitialValue)
	{
		m_pTileMap = new TileType[Size.Y * Size.X];

		for (int nY = 0; nY < Size.Y; nY++)
			for (int nX = 0; nX < Size.X; nX++)
			{
				TileType* pCurrent = getTile(nX, nY);

				pCurrent->initTile(Vector2(nX, nY), getTile(nX - 1, nY), getTile(nX + 1, nY), getTile(nX, nY + 1), getTile(nX, nY - 1));
				pCurrent->setValue(fInitialValue);
			}

	}

	struct sEdge
	{
		Vector2 Start;
		Vector2 End;
	};
	vector<sEdge> vecEdges;

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case MOUSEWORLD_EVENT:
			{
				Vector2 WorldPosition = pEvent->get<MouseWorldEvent>()->getWorldPosition();
				if (WorldPosition.Y >= Position.Y && WorldPosition.Y < Size.Y + Position.Y && WorldPosition.X >= Position.X && WorldPosition.X < Size.X + Position.X)
				{
					if (TileType* pTile = getTile(WorldPosition - Position))
					{
						if (m_pMouseOverTile != pTile)
						{
							//	 new mouseover tile
							if (m_pMouseOverTile != NULL)
							{
								//	previous tile no longer mouse over
								m_pMouseOverTile->setMouseOver(false);
								//	*send previous mouse over tile update event here*
								addEvent(new BaseNodeEvent(m_pMouseOverTile));
							}
							m_pMouseOverTile = pTile;
							m_pMouseOverTile->setMouseOver(true);
							//	*send new mouse over tile event here*
							addEvent(new BaseNodeEvent(m_pMouseOverTile));
						}
					}

				}
				else
				{
					if (m_pMouseOverTile != NULL)
					{
						//	mouse no longer over tilemap at all
						m_pMouseOverTile->setMouseOver(false);
						//	*send previous mouse over tile update event here*
						addEvent(new BaseNodeEvent(m_pMouseOverTile));
						m_pMouseOverTile = NULL;
					}
				}


			}break;

		}
	}

public:
	_TileMap(Vector2 size, string strName) :
		m_pTileMap(NULL),
		m_pMouseOverTile(NULL),
		Size(size),
		Position({ 0, 0 }),
		m_nTotalGroups(0),
		BaseNode(strName)
	{
		registerListener(MOUSEWORLD_EVENT);
		initialize(0.0);
	};

	~_TileMap()
	{
		delete m_pTileMap;
	}

	void setPosition(int x, int y)
	{
		Position.X = x;
		Position.Y = y;
	}

	TileType* getTile(int nX, int nY)
	{
		if (nY <= Size.Y && nX <= Size.X)
			return &m_pTileMap[(nX & int(Size.X) - 1) + int(Size.X) * (nY & int(Size.Y) - 1)];
		else
			return NULL;
	}


	BaseTile* getMouseOverTile()
	{
		return m_pMouseOverTile;
	}

	template <typename T>
	T* getTile(int nX, int nY)
	{
		BaseNode* pParent = getParent();
		while (pParent->isIterating())
		{
			_TileMap* pTileMap = pParent->getCurrent<_TileMap>();
			BaseNode* pTile = pTileMap->getTile(nX, nY);
			if (typeid(*pTile) == typeid(T))
				return static_cast<T*>(pTile);
		}

		return NULL;
	}

	vector<sEdge> getEdges()
	{
		return vecEdges;
	}

	TileType* getTile(Vector2 position)
	{
		if (position.Y >= 0 && position.Y < Size.Y &&
			position.X >= 0 && position.X < Size.X)
			return &m_pTileMap[int(position.X) + int(Size.X) * int(position.Y)];
		else
			return NULL;
	}

	Vector2 getSize() { return Size; }

	//void toString()
	//{
	//	return "[" + toString<int>(Size.X) + "x" + toString<int>(Size.Y) + "]";
	//}

};

class DefaultTileMap :
	public _TileMap<DefaultTile>
{
private:
	int CheckerMap[20][20] = {
								{	0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1	},
								{	1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0	},
								{	0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1	},
								{	1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0	},
								{	0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1	},
								{	1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0	},
								{	0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1	},
								{	1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0	},
								{	0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1	},
								{	1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0	},
								{	0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1	},
								{	1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0	},
								{	0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1	},
								{	1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0	},
								{	0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1	},
								{	1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0	},
								{	0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1	},
								{	1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0	},
								{	0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1	},
								{	1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0	}
	};

	int BoarderMap[20][20] = {
								{	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1	},
								{	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1	},
								{	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1	},
								{	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1	},
								{	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1	},
								{	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1	},
								{	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1	},
								{	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1	},
								{	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1	},
								{	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1	},
								{	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1	},
								{	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1	},
								{	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1	},
								{	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1	},
								{	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1	},
								{	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1	},
								{	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1	},
								{	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1	},
								{	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1	},
								{	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1	}
	};


	void createCheckerMap()
	{
		int color = 0;
		for (int nY = 0; nY < Size.Y; nY++)
		{
			if (color == 0)
				color = 1;
			else
				color = 0;

			for (int nX = 0; nX < Size.X; nX++)
			{
				getTile(nX, nY)->setValue(color);
				if (color == 0)
					color = 1;
				else
					color = 0;
			}
		}
	}

	void createBoarderMap()
	{
		for (int nY = 0; nY < Size.Y; nY++)
			for (int nX = 0; nX < Size.X; nX++)
				if (nY == 0 || nY == Size.Y - 1 || nX == 0 || nX == Size.X - 1)
					getTile(nX, nY)->setValue(1.0);
	}

public:
	DefaultTileMap() :
		_TileMap({ 20, 20 }, "DEFAULT")
	{
		// base.initialize comes before this
		// set tiles to manual mapping
		for (int nY = 0; nY < Size.Y; nY++)
			for (int nX = 0; nX < Size.X; nX++)
			{
				DefaultTile* pTile = getTile(nX, nY);
				pTile->setValue(BoarderMap[nX][nY]);
			}

	};

	DefaultTileMap(int nWidth, int nHeight) :
		_TileMap({ (float)nWidth, (float)nHeight }, "DEFAULT")
	{
		createCheckerMap();
	}

	void render(Render2D *pRenderer, Vector3 vCameraPosition, Vector2 vWorldMin, Vector2 vWorldMax)
	{
		if (vWorldMin.Y < 0)
			vWorldMin.Y = 0;
		if (vWorldMin.X < 0)
			vWorldMin.X = 0;
		if (vWorldMax.Y > Size.Y)
			vWorldMax.Y = Size.Y;
		if (vWorldMax.X > Size.X)
			vWorldMax.X = Size.X;

		int TilesRendered = 0;

		for (int nY = vWorldMin.Y; nY < vWorldMax.Y; nY++)
			for (int nX = vWorldMin.X; nX < vWorldMax.X; nX++)
			{
				Vector2 Min((Position.X + vCameraPosition.X + nX) * vCameraPosition.Z, (Position.Y + vCameraPosition.Y + nY) * vCameraPosition.Z);
				Vector2 Max((Position.X + vCameraPosition.X + nX) * vCameraPosition.Z + 1 * vCameraPosition.Z, (Position.Y + vCameraPosition.Y + nY) * vCameraPosition.Z + 1 * vCameraPosition.Z);

				{
					pRenderer->DrawQuad(Min.X,
						Min.Y,
						Max.X,
						Max.Y,
						pRenderer->getGreyscaleColor(getTile(nX, nY)->getValue()));
				}

				TilesRendered++;

				//	move to bottom of render, to put on top
				if (getTile(nX, nY) == getMouseOverTile())
				{
					pRenderer->DrawQuad(Min.X,
						Min.Y,
						Max.X,
						Max.Y,
						Pixel(PIXEL_SOLID, FG_LIGHTGREEN));//pRenderer->getGreyscaleColor(0));

				}
			}
	}
};