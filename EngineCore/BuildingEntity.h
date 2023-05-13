
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
	_TileMap<BuildingTile>* pRoomMap;
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
		Doors.push_back(new Door(pDoorTile->getLocalPosition().X, pDoorTile->getLocalPosition().Y));
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

	_TileMap<BuildingTile>* getMap()
	{
		return pRoomMap;
	}
};

class Building : public BaseNode, EventListener
{
private:
	Room* current;
	vector<Room*> Rooms;
	Room* m_pMouseOverRoom;
	Vector2 Min;	//	Position
	Vector2 Max;	//	Size

	void onEvent(_Event* pEvent)
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

	Vector2 getMin()
	{
		return Min;
	}
	Vector2 getMax()
	{
		return Max;
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

	void applyToMap(_TileMap<BuildingTile>* map)
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
