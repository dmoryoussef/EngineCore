class CameraWindow :
	public _Window
{
private:
	Render2D  *m_pRenderer;
	BaseNode *m_pCamera;
	Vector2 PreviousMousePosition;
	Vector2 CurrentMousePosition;
	Vector2 vScreenPos;
	bool m_bCameraDrag;
	bool m_bCameraZoom;
	bool m_bDrawMouse;
	bool m_bDrawCameraCoords;

	bool m_bSelectionSquare;
	struct SelectionSquare
	{
		Vector2 Start;
		Vector2 End;
		Vector2 Min;
		Vector2 Max;

		void clear()
		{
			Start = { 0,0 };
			End = { 0,0 };
			Min = { 0,0 };
			Max = { 0,0 };
		}

		void update(Vector2 end)
		{

			End = end;

			Min = Start;
			Max = End;

			if (Start.Y > End.Y)
			{
				if (Start.X > End.X)
				{
					Min = End;
					Max = Start;
				}
				else
				{
					Min.Y = End.Y;
					Min.X = Start.X;

					Max.Y = Start.Y;
					Max.X = End.X;
				}
			}
			else
			{
				if (Start.X > End.X)
				{
					Min.Y = Start.Y;
					Min.X = End.X;

					Max.Y = End.Y;
					Max.X = Start.X;
				}
			}
		}
	} Square;

	string formatMousePosition(Vector3 Position)
	{
		string strPos = toString<int>(Position.trunc().Y) + "x" + toString<int>(Position.trunc().X) + "x" + toString<int>(Position.trunc().Z);
		string strFinal = "";
		strFinal.insert(0, 1, '[');
		strFinal.insert(strFinal.end(), 1, ']');

		strFinal.insert(1, strPos);
		return strFinal;
	}

	string formatMousePosition(Vector2 Position)
	{
		string strPos = toString<int>(Position.trunc().X) + "x" + toString<int>(Position.trunc().Y);
		string strFinal = "";
		strFinal.insert(0, 1, '[');
		strFinal.insert(strFinal.end(), 1, ']');

		strFinal.insert(1, strPos);
		return strFinal;
	}

	Vector2 ScreenPosition(Vector2 WorldPos, Vector2 CameraPos, Vector2 WindowPos)
	{
		return WorldPos + WindowPos + CameraPos;
	}

	Vector2 WorldPosition(Vector2 ScreenPos, Vector2 CameraPos, Vector2 WindowPos, float fScale)
	{
		return (ScreenPos - WindowPos - CameraPos + Vector2(1, 1)) / fScale;	// + (1, 1) corrects for console mouse position somehow :P
	}

	Vector2 WindowPosition(Vector2 ScreenPos, Vector2 WindowPos)
	{
		return ScreenPos - WindowPos;
	}

	void renderCameraView(BaseNode *pGameData, Vector2 vWorldMin, Vector2 vWorldMax)
	{
		m_pCamera->getChild<Camera>()->setView(vWorldMin, vWorldMax);
		pGameData->baseRender(m_pRenderer, m_pCamera);
		
	}

	void drawSquare()
	{
		for (int nY = Square.Min.Y; nY < Square.Max.Y; nY++)
			for (int nX = Square.Min.X; nX < Square.Max.X; nX++)
			{
				if (nY == Square.Min.Y || nY == Square.Max.Y - 1 || nX == Square.Min.X || nX == Square.Max.X - 1)
				{
					set("x", nY, nX, DefaultColor());
				}
			}
	}

	void drawCoordinates()
	{
		Vector3 CameraPosition = m_pCamera->getChild<Transform3D>()->getPosition();

		set("CAMERA " + formatMousePosition(CameraPosition), 1, 1, m_nBGColor + m_nFGColor);
		set("SCALE  " + toString<float>(CameraPosition.Z), 2, 1, m_nBGColor + m_nFGColor);

	}

	void constructComponent(BaseNode* pGameData)
	{
		Vector3 CurrentCameraPosition = m_pCamera->getChild<Transform3D>()->getPosition();
		Vector2 WorldMin = WorldPosition({ 0, 0 }, CurrentCameraPosition.toVec2(), Position, CurrentCameraPosition.Z);
		Vector2 WorldMax = WorldPosition(Size + Position, CurrentCameraPosition.toVec2(), Position, CurrentCameraPosition.Z);
		Vector2 vWorldPosition = WorldPosition(vScreenPos, CurrentCameraPosition.toVec2(), Position, CurrentCameraPosition.Z);


		renderCameraView(pGameData, WorldMin, WorldMax);

		//	optional render overlays
		if (m_bDrawCameraCoords)
			drawCoordinates();

		if (m_bSelectionSquare)
			drawSquare();

		if (m_bMouseOver && m_bDrawMouse)
			set("x", vScreenPos.Y, vScreenPos.X, FG_LIGHTRED + BG_DARKRED);
	}

	void onEvent(_Event* pEvent)
	{
		_Window::onEvent(pEvent);

		switch (pEvent->m_eType)
		{
		case CONSOLE_KEYBOARD_EVENT:
		{
			/*if (pEvent->get<KeyboardEvent>()->getKey() == 'r')
			{
				Vector3 CurrentCameraPosition = m_pCamera->getChild<Transform3D>()->getPosition();
				Vector2 BeforeScaleWorldPosition = WorldPosition(CurrentMousePosition, CurrentCameraPosition.toVec2(), Position, CurrentCameraPosition.Z);

				CurrentCameraPosition.Z = int(1.0);

				Vector2 AfterScaleScaleWorldPosition = WorldPosition(CurrentMousePosition, CurrentCameraPosition.toVec2(), Position, CurrentCameraPosition.Z);
				Vector2 Diff = (BeforeScaleWorldPosition - AfterScaleScaleWorldPosition) * CurrentCameraPosition.Z;

				m_pCamera->getChild<Transform3D>()->setPosition({ CurrentCameraPosition.Y - Diff.Y, CurrentCameraPosition.X - Diff.X, CurrentCameraPosition.Z });
			}
			break;*/
		}
		break;

		case CONSOLE_MOUSE_EVENT:
		{
			Vector3 CurrentCameraPosition = m_pCamera->getChild<Transform3D>()->getPosition();
			MouseState mouseState = pEvent->get<MouseEvent>()->getState();
			CurrentMousePosition = pEvent->get<MouseEvent>()->getState().Position;
			Vector2 worldPosition = WorldPosition(CurrentMousePosition, CurrentCameraPosition.toVec2(), Position, CurrentCameraPosition.Z);

			vScreenPos = CurrentMousePosition - Position;

			if (m_bMouseOver)
				addEvent(new MouseWorldEvent(worldPosition, pEvent->get<MouseEvent>()->getState()));

			// handle scaling
			if (m_bCameraZoom)
			{
				if (pEvent->get<MouseEvent>()->getState().bWheeledDown)
				{
					if (CurrentCameraPosition.Z > 0.50)
					{
						Vector2 BeforeScaleWorldPosition = WorldPosition(CurrentMousePosition, CurrentCameraPosition.toVec2(), Position, CurrentCameraPosition.Z);
						CurrentCameraPosition.Z = CurrentCameraPosition.Z * 0.85;

						Vector2 AfterScaleScaleWorldPosition = WorldPosition(CurrentMousePosition, CurrentCameraPosition.toVec2(), Position, CurrentCameraPosition.Z);
						Vector2 Diff = (BeforeScaleWorldPosition - AfterScaleScaleWorldPosition) * CurrentCameraPosition.Z;

						m_pCamera->getChild<Transform3D>()->setPosition({ CurrentCameraPosition.X - Diff.X, CurrentCameraPosition.Y - Diff.Y, CurrentCameraPosition.Z });
					}
				}

				if (pEvent->get<MouseEvent>()->getState().bWheeledUp)
				{
					if (CurrentCameraPosition.Z < 15.0)
					{
						Vector2 BeforeScaleWorldPosition = WorldPosition(CurrentMousePosition, CurrentCameraPosition.toVec2(), Position, CurrentCameraPosition.Z);
						CurrentCameraPosition.Z = CurrentCameraPosition.Z * 1.15;

						Vector2 AfterScaleScaleWorldPosition = WorldPosition(CurrentMousePosition, CurrentCameraPosition.toVec2(), Position, CurrentCameraPosition.Z);
						Vector2 Diff = (BeforeScaleWorldPosition - AfterScaleScaleWorldPosition) * CurrentCameraPosition.Z;

						m_pCamera->getChild<Transform3D>()->setPosition({ CurrentCameraPosition.X - Diff.X, CurrentCameraPosition.Y - Diff.Y, CurrentCameraPosition.Z });
					}
				}
			}

			// handle dragging
			if (!m_bDragActive)
			{
				{
					bool bMouseDown = pEvent->get<MouseEvent>()->getState().bWheelButtonDown;
					if (!m_bCameraDrag)
					{
						if (bMouseDown)
						{
							if (m_bMouseOver)
							{
								m_bCameraDrag = true;
								PreviousMousePosition = CurrentMousePosition;
							}
						}

					}
					else
					{
						if (bMouseDown)
						{
							Vector2 DeltaPos = PreviousMousePosition - CurrentMousePosition;
							PreviousMousePosition = CurrentMousePosition;
							{
								Vector3 NewPosition = Vector3(CurrentCameraPosition.X - DeltaPos.X,
									CurrentCameraPosition.Y - DeltaPos.Y,
									CurrentCameraPosition.Z);
								m_pCamera->getChild<Transform3D>()->setPosition(NewPosition);
							}
						}
						else
							m_bCameraDrag = false;
					}
				}
			}
			else
			{
				if (m_bCameraDrag)
					m_bDragActive = false;
			}

			// handle selection square
			if (!m_bSelectionSquare)
			{
				if (mouseState.bRightButtonDown)
				{
					Square.Start = mouseState.Position - Vector2(1, 1);
					Square.update(mouseState.Position);
					m_bSelectionSquare = true;
				}
			}
			else
			{
				Square.update(mouseState.Position);
				if (!mouseState.bRightButtonDown)
				{
					//	event
					//	get min/max
					//	make square
					// put in worldPosition
					//	does not work when window is moved
					Vector2 WorldMin = WorldPosition(Square.Min + Vector2(1, 1), CurrentCameraPosition.toVec2(), Position, CurrentCameraPosition.Z);
					Vector2 WorldMax = WorldPosition(Square.Max, CurrentCameraPosition.toVec2(), Position, CurrentCameraPosition.Z);
					//	addEvent(new SelectionSquareEvent(WorldMin, WorldMax));
					m_bSelectionSquare = false;
					Square.clear();
				}
			}

			break;
		}
		}
	}

public:
	CameraWindow(Render2D *pRenderer, BaseNode* pCamera, int nAlignment) :
		m_pRenderer(pRenderer),
		PreviousMousePosition(),
		m_bCameraDrag(false),
		m_bCameraZoom(true),
		m_bSelectionSquare(false),
		m_bDrawMouse(false),
		m_bDrawCameraCoords(false),
		Square(),
		_Window(123, 98, 2, 2)
	{
		m_bDraggable = true;
		m_pCamera = pCamera;
		m_nAlignment = nAlignment;
		setTitle(pCamera->getName());
		m_pRenderer->init(this);
	}

	CameraWindow(Render2D *pRenderer, int nPosX, int nPosY, BaseNode* pCamera, int nAlignment) :
		m_pRenderer(pRenderer),
		m_bCameraZoom(true),
		Square(),
		_Window(123, 98, nPosX, nPosY)
	{
		m_bDraggable = true;
		m_pCamera = pCamera;
		m_nAlignment = nAlignment;
		setTitle(pCamera->getName());

		m_pRenderer->init(this);
	}

};