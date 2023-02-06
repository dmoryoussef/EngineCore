enum BUILDING_TYPES
{
	NONE,
	WALL,
	FLOOR,
	DOOR
};

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
	Vector2 Size;
	Vector2 Position;

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

	void createRoom(Vector2 position, Vector2 size)
	{
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

public:
	Room(Vector2 position, Vector2 size) :
		Position(position),
		Size(size),
		pRoomMap(NULL),
		bMouseOver(false),
		bSelected(false)
	{
		registerListener(MOUSEWORLD_EVENT);
		createRoom(position, size);
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

	Vector2 getPosition() { return Position; }
	Vector2 getSize() { return Size; }

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

class EditableRoom : public EditablePoly2D
{
private:
	//vector<EditableRoom*> EdgeList;	// list of connected rooms - doors

public:
	EditableRoom(Vector2 min, Vector2 max) :
		EditablePoly2D(min, max) {};
		
	void render(Render2D* renderer)
	{
		EditablePoly2D::render(renderer);

		for (auto e : Edges)
		{
			renderer->DrawLine(getMin() + (getSize() / 2), e->getMin() + (e->getSize() / 2), {PIXEL_HALF, FG_LIGHTRED});
		}

		renderer->DrawString("Editable Room", getMin().X + 1, getMin().Y + 1);
	}
};

class Building : public BaseNode, EventListener
{
private:
	Room* current;
	vector<Room*> Rooms;
	Room *m_pMouseOverRoom;
	Vector2 Min;	//	Position
	Vector2 Max;	//	Size

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
						m_pMouseOverRoom = r;
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
		m_pMouseOverRoom(NULL)
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

	void apply(_TileMap<BuildingTile>* map)
	{
		for (auto r : Rooms)
		{
			for (int y = 0; y < (int)r->getSize().Y; y++)
			{
				for (int x = 0; x < (int)r->getSize().X; x++)
				{
					BuildingTile* pCurrentTile = r->getMap()->getTile(x, y);
					Vector2 tileWorldPosition(x + r->getPosition().X, y + r->getPosition().Y);
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

	PolyList<EditableRoom> editableRooms;   

	bool isOverlapping(Vector2 vMin, Vector2 vMax)
	{
		for (auto b : Buildings)
		{
			//	create overlap quad
			//	use to fill invalid tile list
			for (auto r : b->getRooms())
			{
				Vector2 vRoomMin = r->getPosition();
				Vector2 vRoomMax = r->getPosition() + vRoomMin;

				if ((int)vMax.X >= (int)vRoomMin.X - (int)Position.X)
					if ((int)vMin.X <= (int)vRoomMax.X - (int)Position.X)
					{
						//	calc x axis overlap
						if ((int)vMax.Y >= (int)vRoomMin.Y - (int)Position.Y)
							if ((int)vMin.Y <= (int)vRoomMax.Y - (int)Position.Y)
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
		}

		return false;
	}

	Building* getOverlapping(Vector2 vMin, Vector2 vMax)
	{
		//for (auto b : Buildings)
		//{
		//	//	create overlap quad
		//	//	use to fill invalid tile list
		//	if ((int)vMax.X >= (int)b->Min.X - (int)Position.X)
		//		if ((int)vMin.X <= (int)b->Max.X - (int)Position.X)
		//		{
		//			//	calc x axis overlap
		//			if ((int)vMax.Y >= (int)b->Min.Y - (int)Position.Y)
		//				if ((int)vMin.Y <= (int)b->Max.Y - (int)Position.Y)
		//				{
		//					//InvalidTiles.clear();
		//					////	calc y axis overlap
		//					//float fMinX = b->Min.X;
		//					//if (fMinX < vMax.X)
		//					//	fMinX = vMax.X;
		//					//float fMinY = b->Min.Y;
		//					//if (fMinY < vMax.Y)
		//					//	fMinY = vMax.Y;
		//					//Vector2 vOverlapMin(fMinX, fMinY);
		//					//float fMaxX = b->Min.X;
		//					//if (fMinX < vMax.X)
		//					//	fMaxX = vMax.X;
		//					//float fMaxY = b->Min.Y;
		//					//if (fMinY < vMax.Y)
		//					//	fMaxY = vMax.Y;
		//					//Vector2 vOverlapMax(vMax.X, vMax.Y);
		//					//for (int worldY = vOverlapMin.Y; worldY < vOverlapMax.Y; worldY++)
		//					//{
		//					//	for (int worldX = vOverlapMin.X; worldX < vOverlapMax.X; worldX++)
		//					//	{
		//					//		if (BuildingTile* pTile = getWorldTile(worldX, worldY))
		//					//			InvalidTiles.push_back(pTile);
		//					//	}
		//					//}
		//					return b;
		//				}
		//		}
		//}

		return NULL;
	}

	void onGUIEvent(_Event* pEvent)
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

	void onSelectionLineEvent(SelectionLineEvent* pEvent)
	{
		if (EditableRoom *start = editableRooms.getPoly(pEvent->getStart()))
		{
			if (EditableRoom* end = editableRooms.getPoly(pEvent->getStop()))
			{
				start->addEdge(end);
			}
		}
	}

	void onSelectionSquareEvent(_Event* pEvent)
	{
		SelectionSquareEvent* pSelectionEvent = pEvent->get<SelectionSquareEvent>();
		Vector2 vWorldMin = pSelectionEvent->getMin();
		Vector2 vWorldMax = pSelectionEvent->getMax();
		if (pSelectionEvent->isReleased())
		{
			ValidTiles.clear();
			//	draw square
			//	editablePolies.addNewPoly(new EditablePoly2D(vWorldMin, vWorldMax));
			if (nCurrentBuildMode == 1)
			{
				//	if (Building *pBuilding = getOverlapping(vWorldMin - Position, vWorldMax - Position))
				if (isOverlapping(vWorldMin, vWorldMax))
				{
					//	add to existing building
					
					//	pBuilding->addRoom(vWorldMin, vWorldMax);
					//	updateMap(pBuilding);
				}
				else
				{
					//	add new building to list
					// Building* b = new Building(vWorldMin, vWorldMax);
					// Buildings.push_back(b);
					// updateMap(b);	// refresh map with new building data
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

	void onMouseWorldEvent(MouseWorldEvent* pEvent) 
	{
		//	if ()
		//rayEnd = pEvent->getWorldPosition();
		//if (pEvent->getState().bRightButtonDown)
		//{
		//	vIntersection = doRayCast(rayStart, rayEnd);
		//}

		//if (pEvent->getState().bLeftButtonDown)
		//{
		//	Vector2 pos = pEvent->getWorldPosition();
		//	if (BuildingTile* pTile = getWorldTile(pos.X, pos.Y))
		//	{
		//		pTile->setBlocking(true);
		//		pTile->setValue(1.0);
		//	}
		//}
	}

	bool isInList(BuildingTile* tile, vector<BuildingTile*> list)
	{
		for (auto t : list)
		{
			if (tile == t)
			{
				return true;
			}
		}
		return false;
	}

	void updateSingle(Vector2 prevMin, Vector2 prevMax, Vector2 newMin, Vector2 newMax)
	{
		for (int y = prevMin.Y; y <= prevMax.Y; y++)
			for (int x = prevMin.X; x <= prevMax.X; x++)
			{
				if (BuildingTile *pTile = getWorldTile(x, y))
					pTile->setValue(0.1);
			}

		for (int y = newMin.Y; y <= newMax.Y; y++)
			for (int x = newMin.X; x <= newMax.X; x++)
			{
				if (BuildingTile* pTile = getWorldTile(x, y))
				{
					if (x == (int)newMin.X || x == (int)newMax.X || y == (int)newMin.Y || y == (int)newMax.Y)
						pTile->setValue(1.0);
					else
						pTile->setValue(0.4);
				}
			}
	}
	
	void onEditorObjectEvent(EditorObjectEvent* pEvent)
	{
		//	clear all
		for (int i = 0; i < pEvent->getObjects().size(); i = i + 4)
		{
			Vector2 prevMin = pEvent->getObjects()[i];
			Vector2 prevMax = pEvent->getObjects()[i + 1];
			clearBuilding(prevMin, prevMax);
		}
		
		//	apply buildings
		for (int i = 0; i < pEvent->getObjects().size(); i = i + 4)
		{
			Vector2 newMin = pEvent->getObjects()[i + 2];
			Vector2 newMax = pEvent->getObjects()[i + 3];
			setBuilding(newMin, newMax);
		}

	}

	void clearBuilding(Vector2 vWorldMin, Vector2 vWorldMax)
	{
		for (int y = vWorldMin.Y; y <= vWorldMax.Y; y++)
			for (int x = vWorldMin.X; x <= vWorldMax.X; x++)
			{
				if (BuildingTile* pTile = getWorldTile(x, y))
				{
					pTile->setType(NONE);
				}
			}
	}

	void setBuilding(Vector2 vWorldMin, Vector2 vWorldMax)
	{
		for (int y = vWorldMin.Y; y <= vWorldMax.Y; y++)
			for (int x = vWorldMin.X; x <= vWorldMax.X; x++)
			{
				if (BuildingTile* pTile = getWorldTile(x, y))
				{
					if (x == (int)vWorldMin.X || x == (int)vWorldMax.X || y == (int)vWorldMin.Y || y == (int)vWorldMax.Y)
					{
						pTile->setType(WALL);
					}
					else
					{
						pTile->setType(FLOOR);
					}
				}
			}
		//	find doors
		validDoorLocationPerWall(vWorldMin, vWorldMax);
	}

	void validDoorLocationOnWall(Vector2 vWallMin, Vector2 vWallMax, float tA, float tB)
	{
		if (tA >= 0.0 && tB <= 1.0)
		{
			Vector2 vSamplePoint = lerp(vWallMin, vWallMax, tA);
			if (isValidDoorLocation(vSamplePoint.X, vSamplePoint.Y))
			{
				getWorldTile(vSamplePoint.X, vSamplePoint.Y)->setType(DOOR);
			}
			else
			{
				validDoorLocationOnWall(vWallMin, vSamplePoint, tA,  tB + 0.1);

				Vector2 vSamplePoint = lerp(vWallMin, vWallMax, tB);
				if (isValidDoorLocation(vSamplePoint.X, vSamplePoint.Y))
				{
					getWorldTile(vSamplePoint.X, vSamplePoint.Y)->setType(DOOR);
				}
				else
					validDoorLocationOnWall(vSamplePoint, vWallMax, tA - 0.1, tB);
			}
		}
	}

	void validDoorLocationPerWall(Vector2 vBuildingMin, Vector2 vBuildingMax)
	{
		//	top wall
		Vector2 topMin = vBuildingMin;
		Vector2 topMax = { vBuildingMax.X, vBuildingMin.Y };
		validDoorLocationOnWall(topMin, topMax, 0.5, 0.5);

		//	bottom wall
		Vector2 bottomMin = { vBuildingMin.X, vBuildingMax.Y };
		Vector2 bottomMax = vBuildingMax;
		validDoorLocationOnWall(bottomMin, bottomMax, 0.5, 0.5);

		//	left wall
		Vector2 leftMin = vBuildingMin;
		Vector2 leftMax = {vBuildingMin.X, vBuildingMax.Y};
		validDoorLocationOnWall(leftMin, leftMax, 0.5, 0.5);

		//	right wall
		Vector2 rightMin = { vBuildingMax.X, vBuildingMin.Y };
		Vector2 rightMax = vBuildingMax;
		validDoorLocationOnWall(rightMin, rightMax, 0.5, 0.5);
	}

	bool isValidDoorLocation(int x, int y)
	{
		if (getWorldTile(x, y)->getType() == WALL)
		{
			//	already on a wall
			//	find valid door
			//		
			Vector2 vTest(x, y);
			Vector2 vForward(1, 0);
			//Vector2 vSample = vTest + vForward;
			//	sample
			for (int i = 0; i < 4; i++)
			{
				Vector2 vSample = vTest + vForward;

				if (getWorldTile(vSample.X, vSample.Y)->getType() == WALL);
				{
					vForward = vForward.right();
					vForward = vForward.right();
					vSample = vTest + vForward;

					if (getWorldTile(vSample.X, vSample.Y)->getType() == WALL)
					{
						vForward = vForward.right();
						vSample = vTest + vForward;

						if (getWorldTile(vSample.X, vSample.Y)->getType() == NONE || getWorldTile(vSample.X, vSample.Y)->getType() == FLOOR)
						{
							vForward = vForward.right();
							vForward = vForward.right();
							vSample = vTest + vForward;

							if (getWorldTile(vSample.X, vSample.Y)->getType() == NONE || getWorldTile(vSample.X, vSample.Y)->getType() == FLOOR)
							{
								// door
								return true;
							}
						}
					}
				}

				vForward = vForward.right();
			}
		}

		return false;
	}

	void validDoorLocationAll(Vector2 vWorldBuildingMin, Vector2 vWorldBuildingMax)
	{
		for (int y = vWorldBuildingMin.Y; y <= vWorldBuildingMax.Y; y++)
			for (int x = vWorldBuildingMin.X; x <= vWorldBuildingMax.X; x++)
			{
				if (x == (int)vWorldBuildingMin.X || x == (int)vWorldBuildingMax.X || y == (int)vWorldBuildingMin.Y || y == (int)vWorldBuildingMax.Y)
					if (isValidDoorLocation(x, y))
						getWorldTile(x, y)->setType(DOOR);
			}
	}

	void onEvent(_Event *pEvent)
	{
		_TileMap::onEvent(pEvent);

		switch (pEvent->m_eType)
		{
			case GUI_EVENT:				onGUIEvent(pEvent);
				break;
			case MOUSEWORLD_EVENT:		onMouseWorldEvent(pEvent->get<MouseWorldEvent>());
				break;
			case SELECTIONSQUARE_EVENT: onSelectionSquareEvent(pEvent);
				break;
			case EDITOROBJECT_EVENT:	onEditorObjectEvent(pEvent->get<EditorObjectEvent>());
				break;
		}
	}

	void updateMap(Building* b)
	{
		b->apply(this);
	}

public:
	BuildingMap(float width = 100, float height = 100) :
		nCurrentBuildMode(1),
		pSelected(NULL),
		_TileMap({ width, height }, "BUILDING_MAP") 
	{
		setPosition(5, 5);
		
		registerListener(GUI_EVENT);
		registerListener(SELECTIONSQUARE_EVENT);
		registerListener(EDITOROBJECT_EVENT);

		
		for (int nY = 0; nY < Size.Y; nY++)
			for (int nX = 0; nX < Size.X; nX++)
				if (nY == 0 || nY == Size.Y - 1 || nX == 0 || nX == Size.X - 1)
					getTile(nX, nY)->setType(1);
				else
					getTile(nX, nY)->setType(0);
		

	}

	void update(float fDeltaTime)
	{
		editableRooms.update();
	}

	void render(Render2D* pRenderer, Vector3 vCameraPosition, Vector2 vWorldMin, Vector2 vWorldMax)
	{
		Vector2 vTileMin = clipMin(vWorldMin);
		Vector2 vTileMax = clipMax(vWorldMax);

		for (int nY = vTileMin.Y; nY < vTileMax.Y; nY++)
			for (int nX = vTileMin.X; nX < vTileMax.X; nX++)
			{
				Vector2 TileMin = Vector2(nX, nY) + Position;
				Vector2 TileMax = TileMin + Vector2(fTileSize, fTileSize);

				int type = getTile(nX, nY)->getType();
				float color = 0;
				switch (type)
				{
					case NONE: color = 0.0f;
						break;
					case FLOOR: color = 0.4f;
						break;
					case WALL: color = 1.0f;
						break;
					case DOOR: color = 0.5f;
						break;
				}

				pRenderer->FillQuad(TileMin.X,
									TileMin.Y,
									TileMax.X,
									TileMax.Y,
									pRenderer->getGreyscaleColor(color));
			}

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

		//	render valid tile list
		for (auto t : ValidTiles)
		{
			int nX = t->getPosition().X;
			int nY = t->getPosition().Y;
			Vector2 vTileMin = Vector2(nX, nY) + Position;
			Vector2 vTileMax = vTileMin + Vector2(fTileSize, fTileSize);

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
				//buildings are in world space already
				//vScaled functions go from tile space to world space
				//subtract position to normalize

			//Vector2 vOutlineMin = b->Min - Position;
			//Vector2 vOutlineMax = b->Max - Position;
			//Vector2 vTileMin = vScaledMin(vOutlineMin.X, vOutlineMin.Y, vCameraPosition);
			//Vector2 vTileMax = vScaledMax(vOutlineMax.X, vOutlineMax.Y, fScaledTileSize, vCameraPosition);

			//if (b->bMouseOver == true)
			//	pRenderer->DrawQuad(vTileMin.X, vTileMin.Y, vTileMax.X, vTileMax.Y, { PIXEL_SOLID, FG_LIGHTBLUE });

			//if (b->bSelected == true)
			//{
			//	pRenderer->DrawQuad(vTileMin.X, vTileMin.Y, vTileMax.X, vTileMax.Y, { PIXEL_SOLID, FG_LIGHTGREEN });
			//}

			//if (vTileMin.X >= 0 && vTileMax.X < pRenderer->getSize().X &&
			//	vTileMin.Y >= 0 && vTileMax.Y < pRenderer->getSize().Y)
			//{
			//	// currently only renders if building is fully within the screen dimensions
			//	pRenderer->DrawString(thingToString<int>(b->getRooms().size()), vTileMin.X + 2, vTileMin.Y + 2);
			//	pRenderer->DrawString(b->Min.toString(), vTileMin.X + 2, vTileMin.Y + 3);
			//	pRenderer->DrawString(b->Max.toString(), vTileMin.X + 2, vTileMin.Y + 4);
			//	}

		}

		editableRooms.render(pRenderer);
	}

};