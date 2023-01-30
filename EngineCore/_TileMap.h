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
	float fTileSize;

	TileType* m_pMouseOverTile;
	bool m_bMouseOver;



	void initialize(float fInitialValue)
	{
		m_pTileMap = new TileType[(int)Size.Y * (int)Size.X];

		for (int nY = 0; nY < Size.Y; nY++)
			for (int nX = 0; nX < Size.X; nX++)
			{
				TileType* pCurrent = getTile(nX, nY);

				pCurrent->initTile(Vector2(nX, nY), getTile(nX - 1, nY), getTile(nX + 1, nY), getTile(nX, nY + 1), getTile(nX, nY - 1));
				pCurrent->setValue(fInitialValue);
				
			}
	}

	Vector2 clipMin(Vector2 vCameraMin)
	{
		Vector2 vMapMin;

		if (vCameraMin.Y <= Position.Y)
			vMapMin.Y = 0;
		else
			vMapMin.Y = vCameraMin.Y - Position.Y;

		if (vCameraMin.X <= Position.X)
			vMapMin.X = 0;
		else
			vMapMin.X = vCameraMin.X - Position.X;

		return vMapMin;
	}
	
	Vector2 clipMax(Vector2 vCameraMax)
	{
		Vector2 vMapMax;

		if (vCameraMax.Y >= Size.Y + Position.Y)
			vMapMax.Y = Size.Y;
		else
			vMapMax.Y = Size.Y - (Position.Y + Size.Y - vCameraMax.Y);

		if (vCameraMax.X >= Size.X + Position.X)
			vMapMax.X = Size.X;
		else
			vMapMax.X = Size.X - (Position.X + Size.X - vCameraMax.X);

		return vMapMax;
	}

	struct sEdge
	{
		Vector2 Start;
		Vector2 End;
	};
	vector<sEdge> vecEdges;

	void onMouseWorldEvent(MouseWorldEvent* pEvent)
	{
		Vector2 WorldPosition = pEvent->get<MouseWorldEvent>()->getWorldPosition();
		if (WorldPosition.Y >= Position.Y && WorldPosition.Y < Size.Y + Position.Y &&
			WorldPosition.X >= Position.X && WorldPosition.X < Size.X + Position.X)
		{
			m_bMouseOver = true;
			if (TileType* pTile = getWorldTile(WorldPosition.X, WorldPosition.Y))
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
			m_bMouseOver = false;
			if (m_pMouseOverTile != NULL)
			{
				//	mouse no longer over tilemap at all
				m_pMouseOverTile->setMouseOver(false);
				//	*send previous mouse over tile update event here*
				addEvent(new BaseNodeEvent(m_pMouseOverTile));
				m_pMouseOverTile = NULL;
			}
		}
	}

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case MOUSEWORLD_EVENT: onMouseWorldEvent(pEvent->get<MouseWorldEvent>());
				break;
		}
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
				TileType* pCurrent = getTile(nX, nY);
				pCurrent->setValue(color);
				if (color == 0)
					color = 1;
				else
					color = 0;
				//	addEvent(new BaseNodeEvent(pCurrent));
			}
		}
	}

	void createBoarderMap()
	{
		for (int nY = 0; nY < Size.Y; nY++)
			for (int nX = 0; nX < Size.X; nX++)
			{
				getTile(nX, nY)->setValue(0.1);

				if (nY == 0 || nY == Size.Y - 1 || nX == 0 || nX == Size.X - 1)
					m_pTileMap[nX + (int)Size.X * nY].setValue(1.0);

			}
	}

	void render(Render2D* pRenderer, Vector3 vCameraPosition, Vector2 vWorldMin, Vector2 vWorldMax)
	{
		Vector2 vTileMin = clipMin(vWorldMin);
		Vector2 vTileMax = clipMax(vWorldMax);

		float fTileSize = 1.0f;
		int TilesRendered = 0;

		//	render clipped part of map
		for (int nY = vTileMin.Y; nY < vTileMax.Y; nY++)
			for (int nX = vTileMin.X; nX < vTileMax.X; nX++)
			{
				Vector2 TileMin = Vector2(nX, nY) + Position;
				Vector2 TileMax = TileMin + Vector2(fTileSize, fTileSize);

				pRenderer->FillQuad(TileMin.X,
									TileMin.Y,
									TileMax.X,
									TileMax.Y,
									pRenderer->getGreyscaleColor(getTile(nX, nY)->getValue()));

				TilesRendered++;
			}

		if (m_bMouseOver)
		{
			pRenderer->DrawQuad(Position.X, Position.Y, (Position + Size).X, (Position + Size).Y, {PIXEL_SOLID, FG_LIGHTBLUE});
			//pRenderer->DrawLine(Position, { Position.X + Size.X, Position.Y }, { PIXEL_SOLID, FG_LIGHTBLUE });
			//pRenderer->DrawLine(Position, { Position.X, Position.Y + Size.Y }, { PIXEL_SOLID, FG_LIGHTBLUE });
			//pRenderer->DrawLine({ Position.X, Position.Y + Size.Y }, Position + Size, { PIXEL_SOLID, FG_LIGHTBLUE });
			//pRenderer->DrawLine({ Position.X + Size.X, Position.Y }, Position + Size, { PIXEL_SOLID, FG_LIGHTBLUE });
		}

		//	move to bottom of render, to put on top
		if (getMouseOverTile())
		{
			int nX = getMouseOverTile()->getPosition().X + Position.X;
			int nY = getMouseOverTile()->getPosition().Y + Position.X;
			
			Vector2 Min(nX, nY);
			Vector2 Max = Min + Vector2(fTileSize, fTileSize);
			
			pRenderer->DrawQuad(Min.X,
								Min.Y,
								Max.X,
								Max.Y,
								Pixel(PIXEL_SOLID, FG_LIGHTGREEN));
		}

		pRenderer->DrawNum<int>(TilesRendered, 2, pRenderer->getSize().Y - 3, FG_WHITE);


	}

public:
	_TileMap(Vector2 size, string strName) :
		m_pTileMap(NULL),
		m_pMouseOverTile(NULL),
		Size(size),
		Position({ 0, 0 }),
		m_nTotalGroups(0),
		m_bMouseOver(false),
		fTileSize(1.0),
		BaseNode(strName)
	{
		registerListener(MOUSEWORLD_EVENT);
		initialize(0.0);
	};

	~_TileMap()
	{
		delete m_pTileMap;
	}

	Vector2 doRayCast(Vector2 start, Vector2 end)
	{
		https://www.youtube.com/watch?v=NbSee-XM7WA
		Vector2 vRayDir = (end - start).normalize();
		Vector2 vRayStepSize = { sqrt(1 + (vRayDir.Y / vRayDir.X) * (vRayDir.Y / vRayDir.X)), sqrt(1 + (vRayDir.X / vRayDir.Y) * (vRayDir.X / vRayDir.Y)) };

		int startX = (int)start.X;
		int startY = (int)start.Y;
		Vector2 vTileTestPos(startX, startY);

		Vector2 vRayLength1D;
		Vector2 vStep;

		if (vRayDir.X < 0)
		{
			vStep.X = -1;
			vRayLength1D.X = (start.X - vTileTestPos.X) * vRayStepSize.X;
		}
		else
		{
			vStep.X = 1;
			vRayLength1D.X = ((vTileTestPos.X + 1) - start.X) * vRayStepSize.X;
		}

		if (vRayDir.Y < 0)
		{
			vStep.Y = -1;
			vRayLength1D.Y = (start.Y - vTileTestPos.Y) * vRayStepSize.Y;
		}
		else
		{
			vStep.Y = 1;
			vRayLength1D.Y = ((vTileTestPos.Y + 1) - start.Y) * vRayStepSize.Y;
		}

		float fMaxDistance = distance(start, end);
		float fDistance = 0.0f;
		while (fDistance < fMaxDistance)
		{
			if (vRayLength1D.X < vRayLength1D.Y)
			{
				vTileTestPos.X += vStep.X;
				fDistance = vRayLength1D.X;
				vRayLength1D.X += vRayStepSize.X;
			}
			else
			{
				vTileTestPos.Y += vStep.Y;
				fDistance = vRayLength1D.Y;
				vRayLength1D.Y += vRayStepSize.Y;
			}

			if (TileType* pTile = getWorldTile(vTileTestPos.X, vTileTestPos.Y))
			{
				if (pTile->isBlocking())
				{
					return start + vRayDir * fDistance;
				}
			}
		}

		return end;
	}

	void setPosition(int x, int y)
	{
		Position.X = x;
		Position.Y = y;
	}

	TileType* getWorldTile(int nX, int nY)
	{
		//	account for non-zero position in world
		nX = nX - Position.X;
		nY = nY - Position.Y;
		if (nX >= 0 && nY >= 0 && nY <= Size.Y && nX <= Size.X)
			return &m_pTileMap[nX + (int)Size.X * nY];
		else
			return NULL;
	}

	TileType* getTile(int nX, int nY)
	{
		if (nY <= Size.Y && nX <= Size.X)
			return &m_pTileMap[nX + (int)Size.X * nY];
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
	Vector2 getPosition() { return Position; }
	Vector2 getSize() { return Size; }
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
		createCheckerMap();
	}

	void initialize(float* fMap, int w, int h, float scale = 1)
	{
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				DefaultTile* pCurrent = getTile(x, y);

				pCurrent->setValue(fMap[y * w + x]);
			}
		}
	}


};

class OrthographicTileMap : public DefaultTileMap
{

	//	all this needs to be moved to the camera
	//	the map should not know what mode it is in, isographic or orthographic
	//	rendering should be seperate


private:
	Vector2 vTileSize;

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case MOUSEWORLD_EVENT:
			{
				Vector2 vMouse = pEvent->get<MouseWorldEvent>()->getWorldPosition();

				Vector2 vTransformed = orthoTransform(vMouse.X, vMouse.Y);
				if (DefaultTile* pTile = getTile(vTransformed.X, vTransformed.Y))
				{
					if (m_pMouseOverTile != NULL)
						m_pMouseOverTile->setMouseOver(false);
					m_pMouseOverTile = pTile;
					pTile->setMouseOver(true);
				}

				//Vector2 vCell(vMouse.X / vTileSize.X, vMouse.Y / vTileSize.Y);
				////	Vector2 vOffset(vMouse.X % vTileSize.X, vMouse.Y % vTileSize.Y);
				//Vector2 vSelected((vCell.Y - Position.Y) + (vCell.X - Position.X), (vCell.Y - Position.Y) - (vCell.X - Position.X));

				//DefaultTile* pTile = getTile(vSelected.X, vSelected.Y);
				//if (pTile)
				//{
				//	if (m_pMouseOverTile != NULL)
				//		m_pMouseOverTile->setMouseOver(false);
				//	m_pMouseOverTile = pTile;
				//	pTile->setMouseOver(true);
				//}
				break;
			}
		}
	}

public:
	OrthographicTileMap() :
		vTileSize(2, 1),
		DefaultTileMap()
	{
		setPosition(0, 0);
	};

	Vector2 orthoTransform(int x, int y)
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
				Vector2	vWorldCurrent = orthoTransform(x, y);
				Vector2 vWorldScaled = scale(vWorldCurrent, vCameraPosition);

				DefaultTile* pTile = getTile(x, y);

				if (pTile == m_pMouseOverTile)
					pRenderer->DrawCircle(vWorldScaled.X, vWorldScaled.Y, (vTileSize.Y / vTileSize.X) * vCameraPosition.Z, {PIXEL_SOLID, FG_LIGHTGREEN});
				else
					pRenderer->DrawCircle(vWorldScaled.X, vWorldScaled.Y, (vTileSize.Y / vTileSize.X) * vCameraPosition.Z, pRenderer->getGreyscaleColor(pTile->getValue()));

			}
		}

		//	minimap (not in ortho)
		for (int y = 0; y < Size.Y; y++)
			for (int x = 0; x < Size.X; x++)
			{
				if (getTile(x,y) == getMouseOverTile())
					pRenderer->DrawPoint(x + 78, y + 78, pRenderer->getGreyscaleColor(1));
				else
					pRenderer->DrawPoint(x + 78, y + 78, pRenderer->getGreyscaleColor(m_pTileMap[x + (int)Size.X * y].getValue()));
			}
	
	}
};