class CameraViewWindow : public WorldViewWindow
{
private:
	BaseNode *m_pCamera;
	float m_fDeltaTime;
	float m_fTotalTime;
	bool m_bCameraZoomable;
	float fMinZoom;
	float fMaxZoom;
	bool m_bCameraPanning;
	SelectionSquare* pSelectionSquare = NULL;

	bool m_bActiveSelectionSquareKey;
	bool m_bActiveConnectingLine;

	void handleWorldPosition(MouseState mouseState)
	{
		Vector3 vCamera = m_pCamera->getChild<Transform3D>()->getPosition();
		Vector2 worldPosition = WorldPosition(vCurrentMousePosition, vCamera.toVec2(), Position, vCamera.Z);
		addEvent(new MouseWorldEvent(worldPosition, mouseState));
	}

	void handleCameraMove(MouseState mouseState)
	{
		Vector3 vCurrentCameraPosition = m_pCamera->getChild<Transform3D>()->getPosition();
		// handle scaling
		if (m_bCameraZoomable)
		{
			float z = vCurrentCameraPosition.Z;
			if (mouseState.bWheeledDown)	//	zoom out
			{
				z = z * 0.95;
				if (z < fMinZoom)
					z = fMinZoom;
				m_pCamera->getChild<Transform3D>()->setPosition(m_pCamera->getChild<Camera>()->zoom(z, vCurrentMousePosition, vCurrentCameraPosition, Position));
			}

			if (mouseState.bWheeledUp)
			{
				z = z * 1.15;
				if (z > fMaxZoom)
					z = fMaxZoom;

				m_pCamera->getChild<Transform3D>()->setPosition(m_pCamera->getChild<Camera>()->zoom(z, vCurrentMousePosition, vCurrentCameraPosition, Position));
			}
		}

		// handle dragging
		if (!m_bDragActive)	//	prevent camera panning while window is being moved
		{
			bool bMouseDown = mouseState.bWheelButtonDown;
			if (!m_bCameraPanning)
			{
				if (bMouseDown)
				{
					if (m_bMouseOver)
					{
						m_bCameraPanning = true;
						vPreviousMousePosition = vCurrentMousePosition;
					}
				}
			}
			else
			{
				if (bMouseDown)
				{
					Vector2 vDeltaPos = vPreviousMousePosition - vCurrentMousePosition;
					vPreviousMousePosition = vCurrentMousePosition;
					{
						Vector3 vNewPosition = Vector3( vCurrentCameraPosition.X - vDeltaPos.X,
														vCurrentCameraPosition.Y - vDeltaPos.Y,
														vCurrentCameraPosition.Z);
						m_pCamera->getChild<Transform3D>()->setPosition(vNewPosition);
					}
				}
				else
					m_bCameraPanning = false;
			}
		}
		else
		{
			if (m_bCameraPanning)
				m_bDragActive = false;
		}
	}

	void onMouseWorldEvent(MouseWorldEvent* pEvent)
	{
		if (pEvent->getState().bLeftButtonDown && m_bActiveSelectionSquareKey)
		{
			if (!pSelectionSquare)
			{
				pSelectionSquare = new SelectionSquare(pEvent->getWorldPosition());
			}
		}

		if (pEvent->getState().bLeftButtonDown && m_bActiveConnectingLine)
		{
			if (!pSelectionSquare)
			{
				pSelectionSquare = new SelectionSquare(pEvent->getWorldPosition(), true, false);
			}
		}
				
		if (pSelectionSquare)
		{
			if (pSelectionSquare->isReleased())
			{
				delete pSelectionSquare;
				pSelectionSquare = NULL;
			}
		}	
	}

	void onMouseEvent(MouseEvent* pEvent)
	{
		//	UI related events will need this
		//	drag: area, event listener
		//	close: area, event listener, 'x'
		//	resize window, etc
		MouseState mouseState = pEvent->getState();
		vCurrentMousePosition = mouseState.Position;

		if (m_bMouseOver)
		{
			handleWorldPosition(mouseState);
		}
		handleCameraMove(mouseState);
	}

	void onKeyboardEvent(KeyboardEvent* pEvent)
	{
		if (pEvent->getKey() == 'l') m_bActiveConnectingLine = pEvent->isKeyDown();

		if (pEvent->getKey() == 's') m_bActiveSelectionSquareKey = pEvent->isKeyDown();
		
	}

	void onEvent(_Event* pEvent)
	{
		_UIComponent::onEvent(pEvent);

		switch (pEvent->m_eType)
		{
			case MOUSEWORLD_EVENT: onMouseWorldEvent(pEvent->get<MouseWorldEvent>());
				break;
			case CONSOLE_MOUSE_EVENT: onMouseEvent(pEvent->get<MouseEvent>());
				break;
			case KEYBOARD_EVENT: onKeyboardEvent(pEvent->get<KeyboardEvent>());
				break;
		}
	}

	void constructComponent(BaseNode* pBaseNode, OutputBuffer *pBuffer)
	{
		Vector3 vCurrentCameraPosition = m_pCamera->getChild<Transform3D>()->getPosition();
		Render2D renderer(pBuffer, vCurrentCameraPosition);

		Vector2 vWorldMin =		WorldPosition(Position,									vCurrentCameraPosition.toVec2(), Position, vCurrentCameraPosition.Z);
		Vector2 vWorldMax =		WorldPosition(Vector2(m_nWidth, m_nHeight) + Position,	vCurrentCameraPosition.toVec2(), Position, vCurrentCameraPosition.Z);

		//	if a component is implementing render, call it here
		//	good for debugging
		//	tile map rendering happens here
		pBaseNode->baseRender(&renderer, vCurrentCameraPosition, vWorldMin, vWorldMax);

		//	render ecs on top, not called by baseRender...change???
		EntityRenderSystem ecsRenderer;
		ecsRenderer.render(pBaseNode, &renderer, vWorldMin, vWorldMax);

		if (pSelectionSquare)
		{
			pSelectionSquare->render(&renderer);
		}

		pBuffer->set(" Screen: " + vCurrentMousePosition.toString(), getWidth() - 30, 2, FG_WHITE);
		pBuffer->set("  World: " + WorldPosition(vCurrentMousePosition, vCurrentCameraPosition.toVec2(), Position, vCurrentCameraPosition.Z).toString(), getWidth() - 30, 3, FG_WHITE);
		pBuffer->set(" Camera: " + vCurrentCameraPosition.toVec2().toString(), getWidth() - 30, 4, FG_WHITE);
		pBuffer->set("   Zoom: " + thingToString<float>(vCurrentCameraPosition.Z), getWidth() - 30, 5, FG_WHITE);
		pBuffer->set("    FPS: " + thingToString<float>((1 / m_fDeltaTime ) * 1000.0), getWidth() - 30, 6, FG_WHITE);
		pBuffer->set("Runtime: " + thingToString<float>(m_fTotalTime), getWidth() - 30, 7, FG_WHITE);
		pBuffer->set("   WMin: " + vWorldMin.toString(), getWidth() - 30, 8, FG_WHITE);
		pBuffer->set("   WMaX: " + vWorldMax.toString(), getWidth() - 30, 9, FG_WHITE);
		pBuffer->set(" Events: " + thingToString<float>(totalEvents), getWidth() - 30, 10, FG_WHITE);


	}

public:
	CameraViewWindow(int nWidth, int nHeight, int nPosX, int nPosY) :
		m_fTotalTime(0.0),
		m_fDeltaTime(0.0),
		m_bCameraZoomable(true),
		fMinZoom(0.1),
		fMaxZoom(100.0),
	    m_bCameraPanning(false),
		m_bActiveSelectionSquareKey(false),
		m_bActiveConnectingLine(false),
		WorldViewWindow(nWidth, nHeight, nPosX, nPosY)
	{
		m_fScreenScale = 5;
		m_pCamera = new BaseNode("Camera");
		m_pCamera->addNode(new Transform3D({ 0, 0, m_fScreenScale }, { 0, 0, 0 }, { 0, 0, 0 }))->
				   addNode(new Camera());

		registerListener(MOUSEWORLD_EVENT);
		registerListener(KEYBOARD_EVENT);
	}

	void update(float fDeltaTime)
	{
		m_fDeltaTime = fDeltaTime;
		if (fDeltaTime < 10000)	// dt jumps high on load time
			m_fTotalTime = m_fTotalTime + fDeltaTime / 1000;

		Vector3 vCurrentCameraPosition = m_pCamera->getChild<Transform3D>()->getPosition();
		Vector2 vWorldMin = WorldPosition({ 0, 0 }, vCurrentCameraPosition.toVec2(), Position, vCurrentCameraPosition.Z);
		Vector2 vWorldMax = WorldPosition(Vector2(m_nWidth, m_nHeight) + Position, vCurrentCameraPosition.toVec2(), Position, vCurrentCameraPosition.Z);
		m_pCamera->getChild<Camera>()->setWorldMinMax(vWorldMin, vWorldMax);
	}

	BaseNode *getCamera()
	{
		return m_pCamera;
	}
};