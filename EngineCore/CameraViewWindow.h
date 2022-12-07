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

	bool m_bSelectionSquareKeyTriggerPressed;

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
			if (mouseState.bWheeledDown)	//	zoom out
			{
				if (vCurrentCameraPosition.Z > fMinZoom)
				{
					Vector2 vBeforeScaleWorldPosition = WorldPosition(vCurrentMousePosition, vCurrentCameraPosition.toVec2(), Position, vCurrentCameraPosition.Z);
					vCurrentCameraPosition.Z = vCurrentCameraPosition.Z * 0.95;
					if (vCurrentCameraPosition.Z < fMinZoom)
						vCurrentCameraPosition.Z = fMinZoom;	// clamp to min

					Vector2 vAfterScaleScaleWorldPosition = WorldPosition(vCurrentMousePosition, vCurrentCameraPosition.toVec2(), Position, vCurrentCameraPosition.Z);
					Vector2 Diff = (vBeforeScaleWorldPosition - vAfterScaleScaleWorldPosition) * vCurrentCameraPosition.Z;

					m_pCamera->getChild<Transform3D>()->setPosition({ vCurrentCameraPosition.X - Diff.X, vCurrentCameraPosition.Y - Diff.Y, vCurrentCameraPosition.Z });
				}
			}

			if (mouseState.bWheeledUp)
			{
				if (vCurrentCameraPosition.Z < fMaxZoom)	//	zoom in
				{
					Vector2 BeforeScaleWorldPosition = WorldPosition(vCurrentMousePosition, vCurrentCameraPosition.toVec2(), Position, vCurrentCameraPosition.Z);
					vCurrentCameraPosition.Z = vCurrentCameraPosition.Z * 1.15;
					if (vCurrentCameraPosition.Z > fMaxZoom)
						vCurrentCameraPosition.Z = fMaxZoom;	// clamp to max

					Vector2 AfterScaleScaleWorldPosition = WorldPosition(vCurrentMousePosition, vCurrentCameraPosition.toVec2(), Position, vCurrentCameraPosition.Z);
					Vector2 Diff = (BeforeScaleWorldPosition - AfterScaleScaleWorldPosition) * vCurrentCameraPosition.Z;

					m_pCamera->getChild<Transform3D>()->setPosition({ vCurrentCameraPosition.X - Diff.X, vCurrentCameraPosition.Y - Diff.Y, vCurrentCameraPosition.Z });
				}
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

	SelectionSquare* pSelectionSquare = NULL;

	void onMouseWorldEvent(MouseWorldEvent* pEvent)
	{
		if (pEvent->getState().bLeftButtonDown && m_bSelectionSquareKeyTriggerPressed)
		{
			if (!pSelectionSquare)
			{
				pSelectionSquare = new SelectionSquare(pEvent->getWorldPosition());
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
		if (pEvent->getKey() == 's')
		{
			m_bSelectionSquareKeyTriggerPressed = pEvent->isKeyDown();
		}
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
		Vector2 vWorldMin = WorldPosition({ 0, 0 }, vCurrentCameraPosition.toVec2(), Position, vCurrentCameraPosition.Z);
		Vector2 vWorldMax = WorldPosition(Vector2(m_nWidth, m_nHeight) + Position, vCurrentCameraPosition.toVec2(), Position, vCurrentCameraPosition.Z);
		
		//	if a component is implementing render, call it here
		//	good for debugging
		//	tile map rendering happens here
		pBaseNode->baseRender(&renderer, vCurrentCameraPosition, vWorldMin, vWorldMax);

		//	render ecs on top, not called by baseRender...change???
		EntityRenderSystem ecsRenderer;
		ecsRenderer.render(pBaseNode, &renderer, vWorldMin, vWorldMax);

		if (pSelectionSquare)
		{
			pSelectionSquare->render(&renderer, vCurrentCameraPosition);
		}

		pBuffer->set(" Screen: " + vCurrentMousePosition.toString(), getWidth() - 25, 2, FG_WHITE);
		pBuffer->set("  World: " + WorldPosition(vCurrentMousePosition, vCurrentCameraPosition.toVec2(), Position, vCurrentCameraPosition.Z).toString(), getWidth() - 25, 3, FG_WHITE);
		pBuffer->set("   Zoom: " + thingToString<float>(vCurrentCameraPosition.Z), getWidth() - 25, 4, FG_WHITE);
		pBuffer->set("    FPS: " + thingToString<float>((1 / m_fDeltaTime ) * 1000.0), getWidth() - 25, 5, FG_WHITE);
		pBuffer->set("Runtime: " + thingToString<float>(m_fTotalTime), getWidth() - 25, 6, FG_WHITE);
	}

public:
	CameraViewWindow(int nWidth, int nHeight, int nPosX, int nPosY) :
		m_fTotalTime(0.0),
		m_fDeltaTime(0.0),
		m_bCameraZoomable(true),
		fMinZoom(0.1),
		fMaxZoom(25.0),
	    m_bCameraPanning(false),
		m_bSelectionSquareKeyTriggerPressed(false),
		WorldViewWindow(nWidth, nHeight, nPosX, nPosY)
	{
		m_fScreenScale = 5;
		m_pCamera = new BaseNode("Camera");
		m_pCamera->add(new Transform3D({ 0, 0, m_fScreenScale }, { 0, 0, 0 }, { 0, 0, 0 }));

		registerListener(MOUSEWORLD_EVENT);
		registerListener(KEYBOARD_EVENT);
	}

	void update(float fDeltaTime)
	{
		m_fDeltaTime = fDeltaTime;
		if (fDeltaTime < 10000)	// dt jumps high on load time
			m_fTotalTime = m_fTotalTime + fDeltaTime / 1000;
	}

	BaseNode *getCamera()
	{
		return m_pCamera;
	}
};