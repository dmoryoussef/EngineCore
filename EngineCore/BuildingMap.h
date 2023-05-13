enum BUILDING_TYPES
{
	NONE,
	WALL,
	FLOOR,
	DOOR
};

#include "EditableRoom2D.h"
#include "BuildingEntity.h"

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
	Building* pSelected;

	vector<Building*> Buildings;
	PolyList<EditableRoom2D> editableRooms;   


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

		//for (auto b : Buildings)
		//{
		//	if (b->getMin() == vWorldMin && b->getMax() == vWorldMax)
		//		delete b;
		//}
	}
	void updateMap(Building* b)
	{
		b->applyToMap(this);
	}
	void commitBuilding(Vector2 vWorldMin, Vector2 vWorldMax)
	{
		Building* building = new Building(vWorldMin, vWorldMax);
		Buildings.push_back(building);
		createBuilding(vWorldMin, vWorldMax);
	}
	void createBuilding(Vector2 vWorldMin, Vector2 vWorldMax)
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
		//	validDoorLocationPerWall(vWorldMin, vWorldMax);
	}

	void onSelectionLineEvent(SelectionLineEvent* pEvent)
	{
		if (EditableRoom2D *start = editableRooms.getPoly(pEvent->getStart()))
		{
			if (EditableRoom2D* end = editableRooms.getPoly(pEvent->getStop()))
			{
				start->addEdge(end);
			}
		}
	}
	void onKeyboardEvent(KeyboardEvent* pEvent)
	{
		switch (pEvent->getKey())
		{
			case 'd' :
				break;
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
	void onEditorObjectEvent(EditorObjectEvent* pEvent)
	{
		//	clear all
		//for (int i = 0; i < pEvent->getVectors().size(); i = i + 4)
		//{
		//	Vector2 prevMin = pEvent->getVectors()[i];
		//	Vector2 prevMax = pEvent->getVectors()[i + 1];
		//	clearBuilding(prevMin, prevMax);
		//}
		//
		////	apply buildings
		//for (int i = 0; i < pEvent->getVectors().size(); i = i + 4)
		//{
		//	Vector2 newMin = pEvent->getVectors()[i + 2];
		//	Vector2 newMax = pEvent->getVectors()[i + 3];
		//	setBuilding(newMin, newMax);
		//}

		if (BaseNode *b = pEvent->getObjects())
		{
			//	clear temp building
			while (b->isIterating())
			{
				EditableRoom2D* room = dynamic_cast<EditableRoom2D*>(b->getCurrent());
				Vector2 prevMin = room->getPrevMin();
				Vector2 prevMax = room->getPrevMax();
				clearBuilding(prevMin, prevMax);
			}
		}

		if (BaseNode *b = pEvent->getObjects())
		{
			//	draw temp buildings
			while (b->isIterating())
			{
				EditableRoom2D* room = dynamic_cast<EditableRoom2D*>(b->getCurrent());
				Vector2 newMin = room->getMin();
				Vector2 newMax = room->getMax();
				createBuilding(newMin, newMax);
			}
		}
	}

	void onEvent(_Event *pEvent)
	{
		_TileMap::onEvent(pEvent);

		switch (pEvent->m_eType)
		{
			case MOUSEWORLD_EVENT:		onMouseWorldEvent(pEvent->get<MouseWorldEvent>());
				break;
			case SELECTIONSQUARE_EVENT: onSelectionSquareEvent(pEvent);
				break;
			case EDITOROBJECT_EVENT:	onEditorObjectEvent(pEvent->get<EditorObjectEvent>());
				break;
			case KEYBOARD_EVENT:		onKeyboardEvent(pEvent->get<KeyboardEvent>());
				break;
		}
	}

	Building* getBuilding(Vector2 pos)
	{
		for (auto b : Buildings)
			if (isPointvQuad(pos, b->getMin(), b->getMax()))
			{
				return b;
			}

		return NULL;
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
		registerListener(KEYBOARD_EVENT);
		
		// wall the outside boarders of map
		for (int nY = 0; nY < Size.Y; nY++)
			for (int nX = 0; nX < Size.X; nX++)
				if (nY == 0 || nY == Size.Y - 1 || nX == 0 || nX == Size.X - 1)
					getTile(nX, nY)->setType(WALL);
				else
					getTile(nX, nY)->setType(NONE);
		

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
			int nX = getMouseOverTile()->getLocalPosition().X + Position.X;
			int nY = getMouseOverTile()->getLocalPosition().Y + Position.X;

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
			int nX = t->getLocalPosition().X;
			int nY = t->getLocalPosition().Y;
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

		pRenderer->DrawString("Total Buildings: " + thingToString<int>(Buildings.size()), 1, 1);
	}

};