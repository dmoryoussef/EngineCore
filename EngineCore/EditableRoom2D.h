
//class EditableRoom : public EditablePoly2D
//{
//private:
//	int* Tiles;
//	int height;
//	int width;
//	int PosX;
//	int PosY;
//
//	void findDoors()
//	{
//		for (auto e : Edges)
//		{
//			EditableRoom* r = dynamic_cast<EditableRoom*>(e);
//
//			Vector2 aMin = getOutsideMin();
//			Vector2 aMax = getOutsideMax();
//			Vector2 bMin = r->getOutsideMin();
//			Vector2 bMax = r->getOutsideMax();
//			if (isQuadvQuad(aMin, aMax, bMin, bMax) || isQuadvQuad(bMin, bMax, aMin, aMax))
//			{
//				validDoorLocationToRoom(r);
//			}
//		}
//	}
//
//	void validDoorLocationToRoom(EditableRoom* test)
//	{
//		//	check all wall locations in current room
//		for (int y = 0; y < height; y++)
//			for (int x = 0; x < width; x++)
//			{
//				if (Tiles[x + y * width] == WALL)
//				{
//					if (isValidInnerDoorLocation(x, y))
//					{
//
//					}
//				}
//			}
//		//	compare to 
//	}
//
//	bool isValidInnerDoorLocation(int x, int y)
//	{
//		if (Tiles[x + y * width] == WALL)
//		{
//			//	already on a wall
//			//	find valid door
//			Vector2 vTest(x, y);
//			Vector2 vForward(1, 0);
//			//	sample
//			for (int i = 0; i < 4; i++)
//			{
//				Vector2 vSample = vTest + vForward;
//
//				if (Tiles[x + y * width] == WALL);
//				{
//					vForward = vForward.right();
//					vForward = vForward.right();
//					vSample = vTest + vForward;
//
//					if (Tiles[x + y * width] == WALL)
//					{
//						vForward = vForward.right();
//						vSample = vTest + vForward;
//
//						if (Tiles[x + y * width] == FLOOR)
//						{
//							vForward = vForward.right();
//							vForward = vForward.right();
//							vSample = vTest + vForward;
//
//							if (Tiles[x + y * width] == FLOOR)
//							{
//								// valid
//								return true;
//							}
//						}
//					}
//				}
//
//				vForward = vForward.right();
//			}
//		}
//		return false;
//	}
//
//	void validDoorLocationOnWall(Vector2 vWallMin, Vector2 vWallMax, float tA, float tB)
//	{
//		if (tA >= 0.0 && tB <= 1.0)
//		{
//			Vector2 vSamplePoint = lerp(vWallMin, vWallMax, tA);
//			if (isValidDoorLocation(vSamplePoint.X, vSamplePoint.Y))
//			{
//				Tiles[(int)vSamplePoint.X + (int)vSamplePoint.Y * width] = (DOOR);
//			}
//			else
//			{
//				validDoorLocationOnWall(vWallMin, vSamplePoint, tA, tB + 0.1);
//
//				Vector2 vSamplePoint = lerp(vWallMin, vWallMax, tB);
//				if (isValidDoorLocation(vSamplePoint.X, vSamplePoint.Y))
//				{
//					Tiles[(int)vSamplePoint.X + (int)vSamplePoint.Y * width] = (DOOR);
//				}
//				else
//					validDoorLocationOnWall(vSamplePoint, vWallMax, tA - 0.1, tB);
//			}
//		}
//	}
//
//	void validDoorLocationPerWall(Vector2 vBuildingMin, Vector2 vBuildingMax)
//	{
//		//	top wall
//		Vector2 topMin = vBuildingMin;
//		Vector2 topMax = { vBuildingMax.X, vBuildingMin.Y };
//		validDoorLocationOnWall(topMin, topMax, 0.5, 0.5);
//
//		//	bottom wall
//		Vector2 bottomMin = { vBuildingMin.X, vBuildingMax.Y };
//		Vector2 bottomMax = vBuildingMax;
//		validDoorLocationOnWall(bottomMin, bottomMax, 0.5, 0.5);
//
//		//	left wall
//		Vector2 leftMin = vBuildingMin;
//		Vector2 leftMax = { vBuildingMin.X, vBuildingMax.Y };
//		validDoorLocationOnWall(leftMin, leftMax, 0.5, 0.5);
//
//		//	right wall
//		Vector2 rightMin = { vBuildingMax.X, vBuildingMin.Y };
//		Vector2 rightMax = vBuildingMax;
//		validDoorLocationOnWall(rightMin, rightMax, 0.5, 0.5);
//	}
//
//	bool isValidDoorLocation(int x, int y)
//	{
//		if (Tiles[x + y * width] == WALL)
//		{
//			//	already on a wall
//			//	find valid door
//			//		
//			Vector2 vTest(x, y);
//			Vector2 vForward(1, 0);
//			//Vector2 vSample = vTest + vForward;
//			//	sample
//			for (int i = 0; i < 4; i++)
//			{
//				Vector2 vSample = vTest + vForward;
//
//				if (Tiles[x + y * width] == WALL);
//				{
//					vForward = vForward.right();
//					vForward = vForward.right();
//					vSample = vTest + vForward;
//
//					if (Tiles[x + y * width] == WALL)
//					{
//						vForward = vForward.right();
//						vSample = vTest + vForward;
//
//						if (Tiles[x + y * width] == NONE || Tiles[x + y * width] == FLOOR)
//						{
//							vForward = vForward.right();
//							vForward = vForward.right();
//							vSample = vTest + vForward;
//
//							if (Tiles[x + y * width] == NONE || Tiles[x + y * width] == FLOOR)
//							{
//								// door
//								return true;
//							}
//						}
//					}
//				}
//
//				vForward = vForward.right();
//			}
//		}
//
//		return false;
//	}
//
//	void validDoorLocationAll(Vector2 vWorldBuildingMin, Vector2 vWorldBuildingMax)
//	{
//		for (int y = vWorldBuildingMin.Y; y <= vWorldBuildingMax.Y; y++)
//			for (int x = vWorldBuildingMin.X; x <= vWorldBuildingMax.X; x++)
//			{
//				if (x == (int)vWorldBuildingMin.X || x == (int)vWorldBuildingMax.X || y == (int)vWorldBuildingMin.Y || y == (int)vWorldBuildingMax.Y)
//					if (isValidDoorLocation(x, y))
//						Tiles[x + y * width] = (DOOR);
//			}
//	}
//	void onKeyboardEvent(KeyboardEvent* pEvent)
//	{
//		switch (pEvent->getKey())
//		{
//		case 'd':	//	delete
//			break;
//		}
//	}
//	void onEvent(_Event* pEvent)
//	{
//		switch (pEvent->m_eType)
//		{
//		case KEYBOARD_EVENT: onKeyboardEvent(pEvent->get<KeyboardEvent>());
//			break;
//		}
//	}
//
//public:
//	EditableRoom(Vector2 min, Vector2 max) :
//		EditablePoly2D(min, max)
//	{
//		updatePosition({ 0,0 });
//		registerListener(KEYBOARD_EVENT);
//	};
//
//	~EditableRoom()
//	{
//		delete[] Tiles;
//	}
//
//	void defaultLayout()
//	{
//		delete[] Tiles;
//		Tiles = new int[width * height];
//		for (int y = 0; y < height; y++)
//			for (int x = 0; x < width; x++)
//			{
//				if (x == 0 || x == width - 1 || y == 0 || y == height - 1)
//				{
//					Tiles[y * width + x] = WALL;
//				}
//				else
//				{
//					Tiles[y * width + x] = FLOOR;
//				}
//			}
//	}
//	Vector2 getOutsideMin()
//	{
//		return Vector2(PosX, PosY);
//	}
//
//	Vector2 getOutsideMax()
//	{
//		return Vector2(PosX + width, PosY + height);
//	}
//
//	Vector2 getOutsideMin(Vector2 min)
//	{
//		int x = min.X;
//		int y = min.Y;
//		return Vector2(x, y);
//	}
//
//	Vector2 getOutsideMax(Vector2 max)
//	{
//		int x = (int)max.X + 1;
//		int y = (int)max.Y + 1;
//		return Vector2(x, y);
//	}
//
//	void updatePosition(Vector2 mouseDiff)
//	{
//		EditablePoly2D::updatePosition(mouseDiff);
//
//		PosX = getMin().X;
//		PosY = getMin().Y;
//		int maxX = (int)getMax().X + 1;
//		int maxY = (int)getMax().Y + 1;
//		height = maxY - PosY;
//		width = maxX - PosX;
//
//		defaultLayout();
//		findDoors();
//	}
//
//	void render(Render2D* r)
//	{
//		// draw tiles
//		float fTileSize = 1.0f;
//		/*Vector2 vTileMin = clipMin(vWorldMin);
//		Vector2 vTileMax = clipMax(vWorldMax);*/
//
//		for (int nY = 0; nY < height; nY++)
//			for (int nX = 0; nX < width; nX++)
//			{
//				Vector2 TileMin = Vector2(nX, nY) + Vector2(PosX, PosY);
//				Vector2 TileMax = TileMin + Vector2(fTileSize, fTileSize);
//
//				int type = Tiles[nY * width + nX];
//				float color = 0;
//				switch (type)
//				{
//				case NONE: color = 0.0f;
//					break;
//				case FLOOR: color = 0.4f;
//					break;
//				case WALL: color = 1.0f;
//					break;
//				case DOOR: color = 0.5f;
//					break;
//				}
//
//				r->FillQuad(TileMin.X,
//					TileMin.Y,
//					TileMax.X,
//					TileMax.Y,
//					r->getGreyscaleColor(color));
//			}
//
//		//	outline
//		Vector2 start = getOutsideMin();
//		Vector2 end = getOutsideMax();
//		r->DrawLine(start, { end.X, start.Y }, { PIXEL_SOLID, FG_LIGHTGREY });
//		r->DrawLine(start, { start.X, end.Y }, { PIXEL_SOLID, FG_LIGHTGREY });
//		r->DrawLine({ end.X, start.Y }, end, { PIXEL_SOLID, FG_LIGHTGREY });
//		r->DrawLine({ start.X, end.Y }, end, { PIXEL_SOLID, FG_LIGHTGREY });
//
//		EditablePoly2D::render(r);
//
//		for (auto e : Edges)
//		{
//			Pixel color = { PIXEL_HALF, FG_LIGHTRED };
//			EditableRoom* s = dynamic_cast<EditableRoom*>(e);
//			Vector2 aMin = getOutsideMin();
//			Vector2 aMax = getOutsideMax();
//			Vector2 bMin = s->getOutsideMin();
//			Vector2 bMax = s->getOutsideMax();
//
//			if (isQuadvQuad(aMin, aMax, bMin, bMax) || isQuadvQuad(bMin, bMax, aMin, aMax))
//				color = { PIXEL_HALF, FG_LIGHTGREEN };
//			r->DrawLine(getMin() + (getSize() / 2), e->getMin() + (e->getSize() / 2), color);
//		}
//
//		vector<string> data;
//		data.push_back("Editable Room");
//		data.push_back(thingToString<int>(width) + "x" + thingToString<int>(height));
//		r->DrawString(data, getMin().X + 1, getMin().Y + 1);
//	}
//};

class EditableRoom2D : public EditablePoly2D
{
private:
	int* Tiles;
	int height;
	int width;
	int PosX;
	int PosY;

	void onKeyboardEvent(KeyboardEvent* pEvent)
	{
		if (isMouseOver())
		switch (pEvent->getKey())
		{
			case 'd': 
				break;
		}
	}
	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case KEYBOARD_EVENT: onKeyboardEvent(pEvent->get<KeyboardEvent>());
				break;
		}
	}

public:
	EditableRoom2D(Vector2 min, Vector2 max) :
		EditablePoly2D(min, max)
	{
		updatePosition({ 0, 0 });
		registerListener(KEYBOARD_EVENT);
	};

	Vector2 getOutsideMin()
	{
		return Vector2(PosX, PosY);
	}

	Vector2 getOutsideMax()
	{
		return Vector2(PosX + width, PosY + height);
	}

	void updatePosition(Vector2 mouseDiff)
	{
		EditablePoly2D::updatePosition(mouseDiff);

		PosX = getMin().X;
		PosY = getMin().Y;
		int maxX = (int)getMax().X + 1;
		int maxY = (int)getMax().Y + 1;
		height = maxY - PosY;
		width = maxX - PosX;
	}

	void render(Render2D* r)
	{
		//	outline
		Vector2 start = getOutsideMin();
		Vector2 end = getOutsideMax();
		r->DrawLine(start, { end.X, start.Y }, { PIXEL_SOLID, FG_LIGHTGREY });
		r->DrawLine(start, { start.X, end.Y }, { PIXEL_SOLID, FG_LIGHTGREY });
		r->DrawLine({ end.X, start.Y }, end, { PIXEL_SOLID, FG_LIGHTGREY });
		r->DrawLine({ start.X, end.Y }, end, { PIXEL_SOLID, FG_LIGHTGREY });

		EditablePoly2D::render(r);

		for (auto e : Edges)
		{
			Pixel color = { PIXEL_HALF, FG_LIGHTRED };
			EditableRoom2D* s = dynamic_cast<EditableRoom2D*>(e);
			Vector2 aMin = getOutsideMin();
			Vector2 aMax = getOutsideMax();
			Vector2 bMin = s->getOutsideMin();
			Vector2 bMax = s->getOutsideMax();

			if (isQuadvQuad(aMin, aMax, bMin, bMax) || isQuadvQuad(bMin, bMax, aMin, aMax))
				color = { PIXEL_HALF, FG_LIGHTGREEN };
			r->DrawLine(getMin() + (getSize() / 2), e->getMin() + (e->getSize() / 2), color);
		}

		vector<string> data;
		data.push_back("Editable Room");
		data.push_back(thingToString<int>(width) + "x" + thingToString<int>(height));
		data.push_back("");
		if (this->isMouseOver())
		{
			data.push_back("'d' to delete.");
			data.push_back("'c' to commit.");
		}

		r->DrawString(data, getMin().X + 1, getMin().Y + 1);
	}
};