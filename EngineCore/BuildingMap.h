class Door : public BaseNode
{
private:
	bool m_bOpen;

	//	0 = open out
	//		position outside of tile
	//	1 = open in
	//		position inside of tile
	int m_nMountPosition;
	Vector2 vMapPosition;

	bool m_bLocked;
	BaseNode* m_pKey;

public:
	Door(int x, int y) :
		vMapPosition(x, y),
		m_bOpen(false),
		m_bLocked(false),
		m_nMountPosition(0),
		m_pKey(NULL),
		BaseNode("Door") {};
};

class Room : public EventListener
{
private:
	_TileMap<BuildingTile> *pRoomMap;
	vector<Door*> Doors;
	bool bMouseOver;
	bool bSelected;

	void handleMouseWorldEvent(_Event* pEvent)
	{
		MouseWorldEvent* pMouseEvent = pEvent->get<MouseWorldEvent>();

		/*for (auto b : Buildings)
		{
			if (b->bMouseOver && !b->bSelected && pMouseEvent->getState().bLeftButtonDown)
			{
				b->bSelected = true;
				if (pSelected)
					pSelected->bSelected = false;
				pSelected = b;
			}
		}*/

		if (pMouseEvent->getState().bRightButtonDown)
		{
			//	undo build tile

			/*if (BuildingTile* pTile = getWorldTile(pMouseEvent->getWorldPosition().X, pMouseEvent->getWorldPosition().Y))
			{
				if (pTile->getValue() > 0.2)
				{
					pTile->setValue(0.1);
				}
			}*/
		}
	}

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case MOUSEWORLD_EVENT: handleMouseWorldEvent(pEvent);
				break;
		}
	}

public:
	Vector2 Size;
	Vector2 Position;
	Room(Vector2 position, Vector2 size) :
		Position(position),
		Size(size),
		pRoomMap(NULL),
		bMouseOver(false),
		bSelected(false)
	{
		registerListener(MOUSEWORLD_EVENT);

		pRoomMap = new _TileMap<BuildingTile>(size, "map");
		for (int y = 0; y < Size.Y; y++)
		{
			for (int x = 0; x < Size.X; x++)
			{
				if (x == 0 || x == size.X - 1 || y == 0 || y == size.Y - 1)
					//	walls
					pRoomMap->getTile(x, y)->setValue(1.0);
				else
					//	floor
					pRoomMap->getTile(x, y)->setValue(0.4);
			}
		}
		//	door
		BuildingTile* pDoorTile = pRoomMap->getTile(0, size.Y / 2);
		pDoorTile->setValue(0.3);
		Doors.push_back(new Door(pDoorTile->getPosition().X, pDoorTile->getPosition().Y));
	}

	~Room()
	{
		delete pRoomMap;
		for (auto d : Doors)
		{
			delete d;
		}
		Doors.clear();
	}

	bool isMouseOver(Vector2 mouseWorldPosition)
	{
		return (mouseWorldPosition.X > (int)Position.X &&
			mouseWorldPosition.X < (int)Size.X + 1 &&
			mouseWorldPosition.Y >(int)Position.Y &&
			mouseWorldPosition.Y < (int)Size.Y + 1);
	}

	_TileMap<BuildingTile> *getMap()
	{
		return pRoomMap;
	}
};

class Building : public BaseNode, EventListener
{
private:
	Room* current;
	vector<Room*> Rooms;
	Room *m_pMouseOver;

	void onEvent(_Event *pEvent)
	{
		switch (pEvent->m_eType)
		{
			case MOUSEWORLD_EVENT:
			{
				MouseWorldEvent* mouseEvent = pEvent->get<MouseWorldEvent>();
				for (auto r : Rooms)
				{
					if (r->isMouseOver(mouseEvent->getWorldPosition()))
						m_pMouseOver = r;
				}

				break;
			}

			case SELECTIONSQUARE_EVENT:
			{
				/*SelectionSquareEvent* selectionEvent = pEvent->get<SelectionSquareEvent>();
				if (!selectionEvent->isHovering())
				{
					if (bSelected)
					{
						Vector2 vMin = selectionEvent->getMin();
						Vector2 vMax = selectionEvent->getMax();
						for (int y = vMin.Y; y < vMax.Y; y++)
						{
							for (int x = vMin.X; x < vMax.X; x++)
							{
								if (BuildingTile* pTile = getTile(0, x, y))
								{
									OutputDebugStringA(pTile->getPosition().toString().c_str());
									OutputDebugStringA("\n");
								}
							}
						}
					}
				}*/
				break;
			}
		}
	
	}

public:
	Building(Vector2 min, Vector2 max) :
		Min(min.X, min.Y),
		Max((int)max.X, (int)max.Y),
		m_pMouseOver(NULL)
	{
		addRoom(Min, Max);
		registerListener(MOUSEWORLD_EVENT);
		registerListener(SELECTIONSQUARE_EVENT);
	};

	~Building()
	{
		for (auto f : Rooms)
		{
			delete f;
		}

		Rooms.clear();
	}

	Vector2 Min;	//	Position
	Vector2 Max;	//	Size

	void onSelect(bool select)
	{
		//if (bSelected)
		//{
		//		already selected
		//	if (select == false)
		//	{
		//			deselect
		//		bSelected = select;
		//			remove widget
		//	}
		//}
		//else
		//{
		//		not selected
		//	if (select == true)
		//	{
		//			select
		//		bSelected = select;
		//			add widget
		//	}
		//}
	}

	BuildingTile* getTile(int floor, int x, int y)
	{
		int localX = x - (int)Min.X;
		int localY = y - (int)Min.Y;
		return Rooms[floor]->getMap()->getTile(localX, localY);
	}

	void addRoom(Vector2 min, Vector2 max)
	{
		int sizeX = (int)max.X - (int)min.X + 1;
		int sizeY = (int)max.Y - (int)min.Y + 1;
		Rooms.push_back(new Room(min, Vector2(sizeX, sizeY)));
	}

	vector<Room*> getRooms()
	{
		return Rooms;
	}

	void render(_TileMap<BuildingTile>* map)
	{
		for (auto r : Rooms)
		{
			for (int y = 0; y < (int)r->Size.Y; y++)
			{
				for (int x = 0; x < (int)r->Size.X; x++)
				{
					BuildingTile* pCurrentTile = r->getMap()->getTile(x, y);
					Vector2 tileWorldPosition(x + r->Position.X, y + r->Position.Y);
					map->getWorldTile(tileWorldPosition.X, tileWorldPosition.Y)->setValue(pCurrentTile->getValue());
				}
			}
		}
	}
};

class BuildingMap : public _TileMap<BuildingTile>
{
private:
	//	building map specific algorithms
	//		flood fill
	//		tile events?
	//	mouse interation
	//		building, selecting tiles
	//		group selection

	int nCurrentBuildMode;
	vector<BuildingTile*> ValidTiles;
	vector<BuildingTile*> InvalidTiles;
	vector<Building*> Buildings;
	Building* pSelected;

	bool isOverlapping(Vector2 vMin, Vector2 vMax)
	{
		for (auto b : Buildings)
		{
			//	create overlap quad
			//	use to fill invalid tile list
			if ((int)vMax.X >= (int)b->Min.X - (int)Position.X)
				if ((int)vMin.X <= (int)b->Max.X - (int)Position.X)
				{
					//	calc x axis overlap
					if ((int)vMax.Y >= (int)b->Min.Y - (int)Position.Y)
						if ((int)vMin.Y <= (int)b->Max.Y - (int)Position.Y)
						{
							//InvalidTiles.clear();
							////	calc y axis overlap
							//float fMinX = b->Min.X;
							//if (fMinX < vMax.X)
							//	fMinX = vMax.X;
							//float fMinY = b->Min.Y;
							//if (fMinY < vMax.Y)
							//	fMinY = vMax.Y;
							//Vector2 vOverlapMin(fMinX, fMinY);
							//float fMaxX = b->Min.X;
							//if (fMinX < vMax.X)
							//	fMaxX = vMax.X;
							//float fMaxY = b->Min.Y;
							//if (fMinY < vMax.Y)
							//	fMaxY = vMax.Y;
							//Vector2 vOverlapMax(vMax.X, vMax.Y);
							//for (int worldY = vOverlapMin.Y; worldY < vOverlapMax.Y; worldY++)
							//{
							//	for (int worldX = vOverlapMin.X; worldX < vOverlapMax.X; worldX++)
							//	{
							//		if (BuildingTile* pTile = getWorldTile(worldX, worldY))
							//			InvalidTiles.push_back(pTile);
							//	}
							//}
							return true;
						}
				}
		}

		return false;
	}

	Building* getOverlapping(Vector2 vMin, Vector2 vMax)
	{
		for (auto b : Buildings)
		{
			//	create overlap quad
			//	use to fill invalid tile list
			if ((int)vMax.X >= (int)b->Min.X - (int)Position.X)
				if ((int)vMin.X <= (int)b->Max.X - (int)Position.X)
				{
					//	calc x axis overlap
					if ((int)vMax.Y >= (int)b->Min.Y - (int)Position.Y)
						if ((int)vMin.Y <= (int)b->Max.Y - (int)Position.Y)
						{
							//InvalidTiles.clear();
							////	calc y axis overlap
							//float fMinX = b->Min.X;
							//if (fMinX < vMax.X)
							//	fMinX = vMax.X;
							//float fMinY = b->Min.Y;
							//if (fMinY < vMax.Y)
							//	fMinY = vMax.Y;
							//Vector2 vOverlapMin(fMinX, fMinY);
							//float fMaxX = b->Min.X;
							//if (fMinX < vMax.X)
							//	fMaxX = vMax.X;
							//float fMaxY = b->Min.Y;
							//if (fMinY < vMax.Y)
							//	fMaxY = vMax.Y;
							//Vector2 vOverlapMax(vMax.X, vMax.Y);
							//for (int worldY = vOverlapMin.Y; worldY < vOverlapMax.Y; worldY++)
							//{
							//	for (int worldX = vOverlapMin.X; worldX < vOverlapMax.X; worldX++)
							//	{
							//		if (BuildingTile* pTile = getWorldTile(worldX, worldY))
							//			InvalidTiles.push_back(pTile);
							//	}
							//}
							return b;
						}
				}
		}

		return NULL;
	}


	void handleGUIEvent(_Event* pEvent)
	{
		if (pEvent->get<GuiEvent>()->getComponent<UIButton>()->getState() == LEFT_RELEASED)
		{
			if (pEvent->get<GuiEvent>()->getComponent<UIButton>()->getText() == "POINT")
			{
				nCurrentBuildMode = 0;
			}

			if (pEvent->get<GuiEvent>()->getComponent<UIButton>()->getText() == "QUAD")
			{
				nCurrentBuildMode = 1;
			}
		}
	}

	void handleSelectionSquareEvent(_Event* pEvent)
	{
		SelectionSquareEvent* pSelectionEvent = pEvent->get<SelectionSquareEvent>();
		Vector2 vWorldMin = pSelectionEvent->getMin();
		Vector2 vWorldMax = pSelectionEvent->getMax();
		if (!pSelectionEvent->isHovering())
		{
			ValidTiles.clear();
			//	draw square
			if (nCurrentBuildMode == 1)
			{
				if (Building *pBuilding = getOverlapping(vWorldMin - Position, vWorldMax - Position))
				{
					//	add to existing building
					pBuilding->addRoom(vWorldMin, vWorldMax);
					updateMap(pBuilding);
				}
				else
				{
					//	add building to list
					Building* b = new Building(vWorldMin, vWorldMax);
					Buildings.push_back(b);
					updateMap(b);	// refresh map with new building data
				}
			}

		}
		else
		{
			//	square not set/hovering
			ValidTiles.clear();
			for (int y = vWorldMin.Y; y < vWorldMax.Y; y++)
			{
				for (int x = vWorldMin.X; x < vWorldMax.X; x++)
				{
					//	if tile is overlapping building
					//	add to red blocking tiles
					//	else
					//	add to valid tiles
					if (BuildingTile* pTile = getWorldTile(x, y))
						ValidTiles.push_back(pTile);
				}
			}


		}
	}

	void handleMouseWorldEvent(_Event *pEvent) {}

	void onEvent(_Event *pEvent)
	{
		_TileMap::onEvent(pEvent);

		switch (pEvent->m_eType)
		{
			case GUI_EVENT:				handleGUIEvent(pEvent);
				break;

			case MOUSEWORLD_EVENT:		handleMouseWorldEvent(pEvent);
				break;

			case SELECTIONSQUARE_EVENT: handleSelectionSquareEvent(pEvent);
				break;
		}
	}

	void updateMap(Building* b)
	{
		b->render(this);
	}

public:
	BuildingMap() :
		nCurrentBuildMode(1),
		pSelected(NULL),
		_TileMap({ 100, 100 }, "BUILDING_MAP") 
	{
		setPosition(5, 5);
		createBoarderMap();
		registerListener(GUI_EVENT);
		registerListener(SELECTIONSQUARE_EVENT);
	}

	//	move down to _tilemap
	//	or move to renderer
	//	orrrrrr both
	Vector2 vScaledMin(int nX, int nY, Vector3 vCamera)
	{
		return Vector2(vCamera.X + ((nX + Position.X) * vCamera.Z),
			vCamera.Y + ((nY + Position.Y) * vCamera.Z));
	}

	Vector2 vScaledMax(int nX, int nY, float fScaledTile, Vector3 vCamera)
	{
		return Vector2(vCamera.X + ((nX + Position.X) * vCamera.Z) + fScaledTile,
			vCamera.Y + ((nY + Position.Y) * vCamera.Z) + fScaledTile);
	}

	void render(Render2D* pRenderer, Vector3 vCameraPosition, Vector2 vWorldMin, Vector2 vWorldMax)
	{
		float fTileSize = 1.0;
		int TilesRendered = 0;
		float fScaledTileSize = fTileSize * vCameraPosition.Z;


		//	render clipped part of map
		Vector2 vClippedWorldMin = clipMin(vWorldMin);
		Vector2 vClippedWorldMax = clipMax(vWorldMax);
		for (int nY = vClippedWorldMin.Y; nY < vClippedWorldMax.Y; nY++)
			for (int nX = vClippedWorldMin.X; nX < vClippedWorldMax.X; nX++)
			{
				float fTileSize = 1.0;

				Vector2 vTileMin = vScaledMin(nX, nY, vCameraPosition);
				Vector2 vTileMax = vScaledMax(nX, nY, fScaledTileSize, vCameraPosition);

				pRenderer->FillQuad(vTileMin.X,
									vTileMin.Y,
									vTileMax.X,
									vTileMax.Y,
									pRenderer->getGreyscaleColor(getTile(nX, nY)->getValue()));

				TilesRendered++;
			}

		//	move to bottom of render, to put on top
		if (getMouseOverTile())
		{
			int nX = getMouseOverTile()->getPosition().X;
			int nY = getMouseOverTile()->getPosition().Y;
			Vector2 vTileMin = vScaledMin(nX, nY, vCameraPosition);
			Vector2 vTileMax = vScaledMax(nX, nY, fScaledTileSize, vCameraPosition);

			pRenderer->DrawQuad(vTileMin.X,
								vTileMin.Y,
								vTileMax.X,
								vTileMax.Y,
								Pixel(PIXEL_SOLID, FG_LIGHTGREEN));
		}

		//	render valid tile list
		for (auto t : ValidTiles)
		{
			int nX = t->getPosition().X;
			int nY = t->getPosition().Y;
			Vector2 vTileMin = vScaledMin(nX, nY, vCameraPosition);
			Vector2 vTileMax = vScaledMax(nX, nY, fScaledTileSize, vCameraPosition);

			pRenderer->FillQuad(vTileMin.X,
								vTileMin.Y,
								vTileMax.X,
								vTileMax.Y,
								{PIXEL_SOLID, FG_LIGHTGREEN});

			if (vCameraPosition.Z > 4.0)
				pRenderer->DrawQuad(vTileMin.X,
									vTileMin.Y,
									vTileMax.X,
									vTileMax.Y,
									Pixel(PIXEL_SOLID, FG_BLACK));
		}

		/*for (auto t : InvalidTiles)
		{
			int nX = t->getPosition().X;
			int nY = t->getPosition().Y;

			Vector2 Min(vCameraPosition.X + ((nX + Position.X) * vCameraPosition.Z),
				vCameraPosition.Y + ((nY + Position.Y) * vCameraPosition.Z));

			Vector2 Max(vCameraPosition.X + ((nX + Position.X) * vCameraPosition.Z) + fScaledTileSize,
				vCameraPosition.Y + ((nY + Position.Y) * vCameraPosition.Z) + fScaledTileSize);

			pRenderer->FillQuad(Min.X,
				Min.Y,
				Max.X,
				Max.Y,
				{ PIXEL_SOLID, FG_LIGHTRED });


			pRenderer->DrawQuad(Min.X,
				Min.Y,
				Max.X,
				Max.Y,
				Pixel(PIXEL_SOLID, FG_BLACK));
		}*/

		//	render mouse over building outline
		for (auto b : Buildings)
		{
			//	buildings are in world space already
			//	vScaled functions go from tile space to world space
			//	subtract position to normalize
			Vector2 vOutlineMin = b->Min - Position;
			Vector2 vOutlineMax = b->Max - Position;
			Vector2 vTileMin = vScaledMin(vOutlineMin.X, vOutlineMin.Y, vCameraPosition);
			Vector2 vTileMax = vScaledMax(vOutlineMax.X, vOutlineMax.Y, fScaledTileSize, vCameraPosition);

			if (b->bMouseOver == true)
				pRenderer->DrawQuad(vTileMin.X, vTileMin.Y, vTileMax.X, vTileMax.Y, { PIXEL_SOLID, FG_LIGHTBLUE });

			if (b->bSelected == true)
			{
				pRenderer->DrawQuad(vTileMin.X, vTileMin.Y, vTileMax.X, vTileMax.Y, { PIXEL_SOLID, FG_LIGHTGREEN });
			}

			if (vTileMin.X >= 0 && vTileMax.X < pRenderer->getSize().X &&
				vTileMin.Y >= 0 && vTileMax.Y < pRenderer->getSize().Y)
			{
				// currently only renders if building is fully within the screen dimensions
				pRenderer->DrawString(thingToString<int>(b->getRooms().size()), vTileMin.X + 2, vTileMin.Y + 2);
				pRenderer->DrawString(b->Min.toString(), vTileMin.X + 2, vTileMin.Y + 3);
				pRenderer->DrawString(b->Max.toString(), vTileMin.X + 2, vTileMin.Y + 4);
			}

		}

		pRenderer->DrawNum<int>(TilesRendered, 2, pRenderer->getSize().Y - 4, FG_WHITE);
		pRenderer->DrawNum<int>(Buildings.size(), 2, pRenderer->getSize().Y - 3, FG_WHITE);

	}

};