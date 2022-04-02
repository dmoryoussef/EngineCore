class WorldViewWindow : public _Window
{
protected:

	Vector2 ScreenPosition(Vector2 WorldPos, Vector2 CameraPos, Vector2 WindowPos)
	{
		return WorldPos + WindowPos + CameraPos;
	}

	Vector2 WorldPosition(Vector2 ScreenPos, Vector2 CameraPos, Vector2 WindowPos, float fScale)
	{
		return (ScreenPos - WindowPos - CameraPos) / fScale;
	}

	Vector2 WindowPosition(Vector2 ScreenPos, Vector2 WindowPos)
	{
		return ScreenPos - WindowPos;
	}


	void drag(MouseEvent* pMouseEvent)
	{
		if (!m_bDragActive)
		{
			if (m_bMouseOver)	//	or window underneith will be grabbed too
				if (pMouseEvent->getState().Position.Y == getMin().Y
					&& pMouseEvent->getState().Position.X > getMin().X
					&& pMouseEvent->getState().Position.X < getMax().X
					&& pMouseEvent->getState().bLeftButtonDown)
				{
					vStartDrag = vCurrentMousePosition - Position;
					m_bDragActive = true;
				}
		}
		else
		{
			Position = vCurrentMousePosition - vStartDrag;
			if (!pMouseEvent->getState().bLeftButtonDown)
				m_bDragActive = false;
		}
	}

	void onEvent(_Event* pEvent)
	{
		_UIComponent::onEvent(pEvent);

		switch (pEvent->m_eType)
		{
			case CONSOLE_MOUSE_EVENT:
			{

				// convert to component
				//	drag: area, event listener
				//	close: area, event listener, 'x'
				//	resize window, etc
				MouseState mouseState = pEvent->get<MouseEvent>()->getState();
				vCurrentMousePosition = mouseState.Position;


				if (m_bDraggable)
					drag(pEvent->get<MouseEvent>());

				if (m_bMouseOver && pEvent->get<MouseEvent>()->getState().bLeftButtonDown)
					moveToTop();


				Vector2 worldPosition = WorldPosition(vCurrentMousePosition, Vector2(0, 0), Position, m_fScreenScale);

				if (m_bMouseOver)
				{
					addEvent(new MouseWorldEvent(worldPosition, mouseState));

				}


				//// handle selection square
				//if (!m_bSelectionSquare)
				//{
				//	if (mouseState.bRightButtonDown)
				//	{
				//		Square.Start = mouseState.Position - Vector2(1, 1);
				//		Square.update(mouseState.Position);
				//		m_bSelectionSquare = true;
				//	}
				//}
				//else
				//{
				//	Square.update(mouseState.Position);
				//	if (!mouseState.bRightButtonDown)
				//	{
				//		//	event
				//		//	get min/max
				//		//	make square
				//		// put in worldPosition
				//		//	does not work when window is moved
				//		Vector2 WorldMin = WorldPosition(Square.Min + Vector2(1, 1), CurrentCameraPosition.toVec2(), Position, CurrentCameraPosition.Z);
				//		Vector2 WorldMax = WorldPosition(Square.Max, CurrentCameraPosition.toVec2(), Position, CurrentCameraPosition.Z);
				//		addEvent(new SelectionSquareEvent(WorldMin, WorldMax));
				//		m_bSelectionSquare = false;
				//		Square.clear();
				//	}
				//}

				break;
			}

			case BASENODE_EVENT:
			{
				//addChild(new _Window(15, 15, vCurrentMousePosition.X, vCurrentMousePosition.Y));
				break;
			}

		}
	}

	void constructComponent(BaseNode* pBaseNode)
	{
		Render2D* pRenderer = new Render2D(this);
		while (pBaseNode->isIterating())
		{
			pBaseNode->getCurrent()->render(pRenderer, Vector3(0, 0, 5), Vector2(0, 0), Vector2(getWidth(), getHeight()));
		}

		set("Mouse: " + vCurrentMousePosition.toString(), getWidth() - 20, 2, FG_WHITE);
		set("World: " + WorldPosition(vCurrentMousePosition, Vector2(0, 0), Position, m_fScreenScale).toString(), getWidth() - 20, 3, FG_WHITE);
	}

public:
	WorldViewWindow(int nWidth, int nHeight, int nPosX, int nPosY) :
		_Window(nWidth, nHeight, nPosX, nPosY)
	{
		registerListener(BASENODE_EVENT);
	}
};