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
				if (WorldPosition.Y >= Position.Y && WorldPosition.Y < Size.Y + Position.Y && 
					WorldPosition.X >= Position.X && WorldPosition.X < Size.X + Position.X)
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



	void createBoarderMap()
	{
		for (int nY = 0; nY < Size.Y; nY++)
			for (int nX = 0; nX < Size.X; nX++)
			{
				m_pTileMap[nX + (int)Size.X * nY].setValue(0.1);

				if (nY == 0 || nY == Size.Y - 1 || nX == 0 || nX == Size.X - 1)
					m_pTileMap[nX + (int)Size.X * nY].setValue(1.0); 

			}
	}

public:
	DefaultTileMap() :
		_TileMap({ 20, 20 }, "DEFAULT")
	{
		// base.initialize comes before this
		// set tiles to manual mapping


		createBoarderMap();

	};

	DefaultTileMap(int nWidth, int nHeight) :
		_TileMap({ (float)nWidth, (float)nHeight }, "DEFAULT")
	{
		//	createCheckerMap();
	}

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
				DefaultTile* pCurrent = getTile(nX, nY);
				pCurrent->setValue(color);
				if (color == 0)
					color = 1;
				else
					color = 0;
				//	addEvent(new BaseNodeEvent(pCurrent));
			}
		}
	}
	void render(Render2D *pRenderer, Vector3 vCameraPosition, Vector2 vWorldMin, Vector2 vWorldMax)
	{
	
		Vector2 vTileMin;
		Vector2 vTileMax;
		
		//	handle clipping
		if (vWorldMin.Y <= Position.Y)
			vTileMin.Y = 0;
		else
			vTileMin.Y = vWorldMin.Y - Position.Y;

		if (vWorldMin.X <= Position.X)
			vTileMin.X = 0;
		else
			vTileMin.X = vWorldMin.X - Position.X;

		if (vWorldMax.Y >= Size.Y + Position.Y)
			vTileMax.Y = Size.Y;
		else
			vTileMax.Y = Size.Y - (Position.Y + Size.Y - vWorldMax.Y);

		if (vWorldMax.X >= Size.X + Position.X)
			vTileMax.X = Size.X;
		else
			vTileMax.X = Size.X - (Position.X + Size.X - vWorldMax.X);

		int TilesRendered = 0;


		//	render clipped part of map
		for (int nY = vTileMin.Y; nY < vTileMax.Y; nY++)
			for (int nX = vTileMin.X; nX < vTileMax.X; nX++)
			{
				float fTileSize = 1.0;
				float fScaledTileSize = fTileSize * vCameraPosition.Z;

				Vector2 Min(vCameraPosition.X + ((nX + Position.X) * vCameraPosition.Z),
							vCameraPosition.Y + ((nY + Position.Y) * vCameraPosition.Z));

				Vector2 Max(vCameraPosition.X + ((nX + Position.X) * vCameraPosition.Z) + fScaledTileSize,
							vCameraPosition.Y + ((nY + Position.Y) * vCameraPosition.Z) + fScaledTileSize);

				
				pRenderer->DrawQuad(Min.X,
									Min.Y,
									Max.X,
									Max.Y,
									pRenderer->getGreyscaleColor(getTile(nX, nY)->getValue()));
				

				TilesRendered++;

				//	move to bottom of render, to put on top
				if (getTile(nX, nY) == getMouseOverTile())
				{
					pRenderer->DrawQuad(Min.X,
									Min.Y,
									Max.X,
									Max.Y,
									Pixel(PIXEL_SOLID, FG_LIGHTGREEN));

				}
			}

		pRenderer->DrawNum<int>(TilesRendered, 2, pRenderer->getSize().Y - 3, FG_WHITE);
	}
};


//template <typename TileType> class _TileMap :
//	public BaseNode,
//	public EventListener


class OrthographicTileMap : public DefaultTileMap
{
private:
	Vector2 vTileSize;

public:
	OrthographicTileMap() :
		vTileSize(2, 1),
		DefaultTileMap()
	{
		setPosition(0, 0);
	};

	Vector2 toScreen(int x, int y)
	{
		float screenX = (Position.X * vTileSize.X) + (x - y) * (vTileSize.X / 2);
		float screenY = (Position.Y * vTileSize.Y) + (x + y) * (vTileSize.Y / 2);
		return Vector2(screenX, screenY);
	}

	Vector2 scale(Vector2 current, Vector3 vCameraPosition)
	{
		return Vector2(vCameraPosition.X + ((current.X + Position.X) * vCameraPosition.Z),
			vCameraPosition.Y + ((current.Y + Position.Y) * vCameraPosition.Z));
	}

	void render(Render2D* pRenderer, Vector3 vCameraPosition, Vector2 vWorldMin, Vector2 vWorldMax)
	{
		for (int y = 0; y < Size.Y; y++)
		{
			for (int x = 0; x < Size.X; x++)
			{
				Vector2	vWorldCurrent = toScreen(x, y);
				Vector2 vWorldScaled = scale(vWorldCurrent, vCameraPosition);

				DefaultTile* pTile = &m_pTileMap[x + (int)Size.X * y];


				pRenderer->DrawCircle(vWorldScaled.X, vWorldScaled.Y, (vTileSize.Y / vTileSize.X) * vCameraPosition.Z, pRenderer->getGreyscaleColor(pTile->getValue()));
				
				//if (getTile(x, y) == getMouseOverTile())
				//	pRenderer->DrawCircle(vWorldScaled.X, vWorldScaled.Y, (vTileSize.Y / vTileSize.X) * vCameraPosition.Z, { PIXEL_SOLID, FG_LIGHTGREEN });

			}
		}

		for (int y = 0; y < Size.Y; y++)
		
			for (int x = 0; x < Size.X; x++)
			{
				pRenderer->DrawPoint(x + 78, y + 78, pRenderer->getGreyscaleColor(m_pTileMap[x + (int)Size.X * y].getValue()));
			}
	
	}
};