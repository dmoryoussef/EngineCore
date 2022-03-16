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

	void toPolyAlgorythm()
	{
		//  TODO:: add world min and world max to only do a part of the whole map	
		//	clear vector
		vecEdges.clear();
		for (int nY = 0; nY < Size.Y; nY++)
			for (int nX = 0; nX < Size.X; nX++)
			{
				TileType* pCurrent = getTile(nY, nX);
				pCurrent->getEdges().reset();
			}
		//	go through tiles
		for (int nY = 0; nY < Size.Y; nY++)
			for (int nX = 0; nX < Size.X; nX++)
			{
				TileType* pTile = getTile(nY, nX);
				//	if cell "exists"
				if (pTile->getValue() > 0.0)
				{
					//	check west tile
					if (TileType* pWest = pTile->getWest())
					{
						// if tile doesnt exist
						if (pWest->getValue() == 0.0)
						{
							//	create or extend edge
							if (TileType* pNorth = pTile->getNorth())
							{
								//	if northern western edge can be exended
								if (pNorth->getEdges().bWestExists)
								{
									// extend edge down
									int nId = pNorth->getEdges().nEdgeID[WEST];
									vecEdges[nId].End.Y += 1;
									// current tile also has western else, id == northern western edge id
									pTile->getEdges().bWestExists = true;
									pTile->getEdges().nEdgeID[WEST] = nId;
								}
								else
								{
									//	new edge
									sEdge edge;
									edge.Start.Y = pTile->getPosition().Y; edge.Start.X = pTile->getPosition().X;
									edge.End.Y = pTile->getPosition().Y + 1; edge.End.X = pTile->getPosition().X;
									int nId = vecEdges.size();
									vecEdges.push_back(edge);
									pTile->getEdges().nEdgeID[WEST] = nId;
									pTile->getEdges().bWestExists = true;
								}
							}
						}
						else
						{
							//	else has neighbor
							//	do nothing
						}

					}
					else
					{
						// edge of map - no western edge
						if (TileType* pNorth = pTile->getNorth())
						{
							//	if northern western edge can be exended
							if (pNorth->getEdges().bWestExists)
							{
								// extend edge down
								int nId = pNorth->getEdges().nEdgeID[WEST];
								vecEdges[nId].End.Y += 1;
								// current tile also has western else, id == northern western edge id
								pTile->getEdges().bWestExists = true;
								pTile->getEdges().nEdgeID[WEST] = nId;
							}
							else
							{
								//	new edge
								sEdge edge;
								edge.Start.Y = pTile->getPosition().Y; edge.Start.X = pTile->getPosition().X;
								edge.End.Y = pTile->getPosition().Y + 1; edge.End.X = pTile->getPosition().X;
								int nId = vecEdges.size();
								vecEdges.push_back(edge);
								pTile->getEdges().nEdgeID[WEST] = nId;
								pTile->getEdges().bWestExists = true;
							}
						}
						else
						{
							//	edge of map no northern edge
							//	new edge
							sEdge edge;
							edge.Start.Y = pTile->getPosition().Y; edge.Start.X = pTile->getPosition().X;
							edge.End.Y = pTile->getPosition().Y + 1; edge.End.X = pTile->getPosition().X;
							int nId = vecEdges.size();
							vecEdges.push_back(edge);
							pTile->getEdges().nEdgeID[WEST] = nId;
							pTile->getEdges().bWestExists = true;
							pTile->getEdges().bWestExists;
						}
					}
					//	check east
					if (TileType* pEast = pTile->getEast())
					{
						if (pEast->getValue() == 0.0)
						{
							//	create or extend edge
							//	if northern edge can be exended
							if (TileType* pNorth = pTile->getNorth())
							{
								if (pNorth->getEdges().bEastExists)
								{
									int nId = pNorth->getEdges().nEdgeID[EAST];
									vecEdges[nId].End.Y = pTile->getPosition().Y + 1;
									pTile->getEdges().bEastExists = true;
									pTile->getEdges().nEdgeID[EAST] = nId;
								}
								else
								{
									//	new edge
									sEdge edge;
									edge.Start.Y = pTile->getPosition().Y;
									edge.Start.X = pTile->getPosition().X + 1;

									edge.End.Y = pTile->getPosition().Y + 1;
									edge.End.X = pTile->getPosition().X + 1;

									int nId = vecEdges.size();
									vecEdges.push_back(edge);
									pTile->getEdges().nEdgeID[EAST] = nId;
									pTile->getEdges().bEastExists = true;
								}
							}
						}
						else
						{
							//	else has neighbor
							//	do nothing
						}
					}
					else
					{
						//	eastern edge of map
						if (TileType* pNorth = pTile->getNorth())
						{
							if (pNorth->getEdges().bEastExists)
							{
								int nId = pNorth->getEdges().nEdgeID[EAST];
								vecEdges[nId].End.Y += 1;
								pTile->getEdges().bEastExists = true;
								pTile->getEdges().nEdgeID[EAST] = nId;
							}
							else
							{
								//	new edge
								sEdge edge;
								edge.Start.Y = pTile->getPosition().Y;
								edge.Start.X = pTile->getPosition().X + 1;

								edge.End.Y = pTile->getPosition().Y + 1;
								edge.End.X = pTile->getPosition().X + 1;
								int nId = vecEdges.size();
								vecEdges.push_back(edge);
								pTile->getEdges().nEdgeID[EAST] = nId;
								pTile->getEdges().bEastExists = true;
							}
						}
						else
						{
							//	new edge
							sEdge edge;
							edge.Start.Y = pTile->getPosition().Y; edge.Start.X = pTile->getPosition().X + 1;
							edge.End.Y = pTile->getPosition().Y + 1; edge.End.X = pTile->getPosition().X + 1;
							int nId = vecEdges.size();
							vecEdges.push_back(edge);
							pTile->getEdges().nEdgeID[EAST] = nId;
							pTile->getEdges().bEastExists = true;
						}
					}

					//	check north
					if (TileType* pNorth = pTile->getNorth())
					{
						if (pNorth->getValue() == 0.0)
						{
							//	create or extend edge
							//	if northern edge can be exended
							if (TileType* pWest = pTile->getWest())
							{
								if (pWest->getEdges().bNorthExists)
								{
									int nId = pWest->getEdges().nEdgeID[NORTH];
									vecEdges[nId].End.X += 1;
									pTile->getEdges().bNorthExists = true;
									pTile->getEdges().nEdgeID[NORTH] = nId;
								}
								else
								{
									//	new edge
									sEdge edge;
									edge.Start.Y = pTile->getPosition().Y; edge.Start.X = pTile->getPosition().X;
									edge.End.Y = pTile->getPosition().Y; edge.End.X = pTile->getPosition().X + 1;
									int nId = vecEdges.size();
									vecEdges.push_back(edge);
									pTile->getEdges().nEdgeID[NORTH] = nId;
									pTile->getEdges().bNorthExists = true;
								}
							}
						}
						else
						{
							//	else has neighbor
							//	do nothing
						}
					}
					else
					{
						if (TileType* pWest = pTile->getWest())
						{
							if (pWest->getEdges().bNorthExists)
							{
								int nId = pWest->getEdges().nEdgeID[NORTH];
								vecEdges[nId].End.X += 1;
								pTile->getEdges().bNorthExists = true;
								pTile->getEdges().nEdgeID[NORTH] = nId;
							}
							else
							{
								//	new edge
								sEdge edge;
								edge.Start.Y = pTile->getPosition().Y; edge.Start.X = pTile->getPosition().X;
								edge.End.Y = pTile->getPosition().Y; edge.End.X = pTile->getPosition().X + 1;
								int nId = vecEdges.size();
								vecEdges.push_back(edge);
								pTile->getEdges().nEdgeID[NORTH] = nId;
								pTile->getEdges().bNorthExists = true;
							}
						}
						else
						{
							//	new edge
							sEdge edge;
							edge.Start.Y = pTile->getPosition().Y; edge.Start.X = pTile->getPosition().X;
							edge.End.Y = pTile->getPosition().Y; edge.End.X = pTile->getPosition().X + 1;
							int nId = vecEdges.size();
							vecEdges.push_back(edge);
							pTile->getEdges().nEdgeID[NORTH] = nId;
							pTile->getEdges().bNorthExists = true;
						}
					}

					//	check south
					if (TileType* pSouth = pTile->getSouth())
					{
						if (pSouth->getValue() == 0.0)
						{
							//	create or extend edge
							//	if northern edge can be exended
							if (TileType* pWest = pTile->getWest())
							{
								if (pWest->getEdges().bSouthExists)
								{
									int nId = pWest->getEdges().nEdgeID[SOUTH];
									vecEdges[nId].End.X += 1;
									pTile->getEdges().bSouthExists = true;
									pTile->getEdges().nEdgeID[SOUTH] = nId;
								}
								else
								{
									//	new edge
									sEdge edge;
									edge.Start.Y = pTile->getPosition().Y + 1; edge.Start.X = pTile->getPosition().X;
									edge.End.Y = pTile->getPosition().Y + 1; edge.End.X = pTile->getPosition().X + 1;
									int nId = vecEdges.size();
									vecEdges.push_back(edge);
									pTile->getEdges().nEdgeID[SOUTH] = nId;
									pTile->getEdges().bSouthExists = true;
								}
							}
						}
						else
						{
							//	else has neighbor
							//	do nothing
						}
					}
					else
					{
						if (TileType* pWest = pTile->getWest())
						{
							if (pWest->getEdges().bSouthExists)
							{
								int nId = pWest->getEdges().nEdgeID[SOUTH];
								vecEdges[nId].End.X += 1;
								pTile->getEdges().bSouthExists = true;
								pTile->getEdges().nEdgeID[SOUTH] = nId;
							}
							else
							{
								//	new edge
								sEdge edge;
								edge.Start.Y = pTile->getPosition().Y + 1; edge.Start.X = pTile->getPosition().X;
								edge.End.Y = pTile->getPosition().Y + 1; edge.End.X = pTile->getPosition().X + 1;
								int nId = vecEdges.size();
								vecEdges.push_back(edge);
								pTile->getEdges().nEdgeID[SOUTH] = nId;
								pTile->getEdges().bSouthExists = true;
							}
						}
						else
						{
							//	new edge
							sEdge edge;
							edge.Start.Y = pTile->getPosition().Y + 1; edge.Start.X = pTile->getPosition().X;
							edge.End.Y = pTile->getPosition().Y + 1; edge.End.X = pTile->getPosition().X + 1;
							int nId = vecEdges.size();
							vecEdges.push_back(edge);
							pTile->getEdges().nEdgeID[SOUTH] = nId;
							pTile->getEdges().bSouthExists = true;
						}
					}
				}
			}

	}

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case MOUSEWORLD_EVENT:
			{
				Vector2 Position = pEvent->get<MouseWorldEvent>()->getWorldPosition();
				if (Position.Y >= 0 && Position.Y < Size.Y && Position.X >= 0 && Position.X < Size.X)
				{
					if (TileType* pTile = getTile(Position))
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


					/*MouseState State = pEvent->get<MouseWorldEvent>()->getState();
					if (State.bLeftButtonDown)
					{
						getTile(Position)->setValue(1.0);
						toPolyAlgorythm();
					}
					if (State.bRightButtonDown)
					{
						getTile(Position)->setValue(0.0);
						toPolyAlgorythm();
					}*/
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
		registerListener(this, MOUSEWORLD_EVENT);
		initialize(0.0);
	};
	~_TileMap()
	{
		delete m_pTileMap;
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

		toPolyAlgorythm();
	};

	DefaultTileMap(int nWidth, int nHeight) :
		_TileMap({ (float)nWidth, (float)nHeight }, "DEFAULT")
	{
		createCheckerMap();
	}

	void render(Render2D *pRenderer, Vector3 vCameraPosition, Vector2 vWorldMin, Vector2 vWorldMax)
	{
		/*Vector3 vCameraPosition = pCamera->getChild<Transform3D>()->getPosition();
		Vector2 vWorldMin = pCamera->getChild<Camera>()->getWorldMin();
		Vector2 vWorldMax = pCamera->getChild<Camera>()->getWorldMax();*/

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
				Vector2 Min(vCameraPosition.X + nX * vCameraPosition.Z, vCameraPosition.Y + nY * vCameraPosition.Z);
				Vector2 Max(vCameraPosition.X + nX * vCameraPosition.Z + 1 * vCameraPosition.Z, vCameraPosition.Y + nY * vCameraPosition.Z + 1 * vCameraPosition.Z);

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