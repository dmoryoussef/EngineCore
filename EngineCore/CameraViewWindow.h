class CameraViewWindow : public WorldViewWindow
{
private:
	BaseNode *m_pCamera;
	bool m_bCameraZoomable;
	float fMinZoom;
	float fMaxZoom;
	bool m_bCameraPanning;

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

	void onEvent(_Event* pEvent)
	{
		_UIComponent::onEvent(pEvent);

		switch (pEvent->m_eType)
		{
			case MOUSEWORLD_EVENT:
			{
				if (pEvent->get<MouseWorldEvent>()->getState().bLeftButtonDown)
				{
					if (!pSelectionSquare)
						pSelectionSquare = new SelectionSquare(pEvent->get<MouseWorldEvent>()->getWorldPosition());
				}
				
				if (pSelectionSquare)
				{
					if (!pSelectionSquare->isActive())
					{
						delete pSelectionSquare;
						pSelectionSquare = NULL;
					}
				}
				
				break;
			}

			case CONSOLE_MOUSE_EVENT:
			{
				// convert to component??
				//	drag: area, event listener
				//	close: area, event listener, 'x'
				//	resize window, etc
				MouseState mouseState = pEvent->get<MouseEvent>()->getState();
				vCurrentMousePosition = mouseState.Position;

				if (m_bMouseOver)
				{
					handleWorldPosition(mouseState);
				}
				handleCameraMove(mouseState);

				break;
			}
		}
	}

	void constructComponent(BaseNode* pBaseNode)
	{
		Render2D renderer(this);
		Vector3 vCurrentCameraPosition = m_pCamera->getChild<Transform3D>()->getPosition();
		Vector2 vWorldMin = WorldPosition({ 0, 0 }, vCurrentCameraPosition.toVec2(), Position, vCurrentCameraPosition.Z);
		Vector2 vWorldMax = WorldPosition(Size + Position, vCurrentCameraPosition.toVec2(), Position, vCurrentCameraPosition.Z);
		
		//	if a component is implementing render, call it here
		//	good for debugging
		//	tile map rendering happens here
		pBaseNode->baseRender(&renderer, vCurrentCameraPosition, vWorldMin, vWorldMax);

		//	render ecs on top, not called by baseRender...change???
		EntityRenderSystem ecsRenderer;
		ecsRenderer.render(pBaseNode, &renderer, vCurrentCameraPosition, vWorldMin, vWorldMax);

		if (pSelectionSquare)
		{
			pSelectionSquare->render(&renderer, vCurrentCameraPosition);
		}

		set("Screen: " + vCurrentMousePosition.toString(), getWidth() - 20, 2, FG_WHITE);
		set("World:  " + WorldPosition(vCurrentMousePosition, vCurrentCameraPosition.toVec2(), Position, vCurrentCameraPosition.Z).toString(), getWidth() - 20, 3, FG_WHITE);	
	}

public:
	CameraViewWindow(int nWidth, int nHeight, int nPosX, int nPosY) :
		m_bCameraZoomable(true),
		fMinZoom(0.1),
		fMaxZoom(15.0),
	    m_bCameraPanning(false),
		WorldViewWindow(nWidth, nHeight, nPosX, nPosY)
	{
		m_fScreenScale = 5;
		m_pCamera = new BaseNode("Camera");
		m_pCamera->add(new Transform3D({ 0, 0, m_fScreenScale }, { 0, 0, 0 }, { 0, 0, 0 }));

		registerListener(MOUSEWORLD_EVENT);
	}

	BaseNode *getCamera()
	{
		return m_pCamera;
	}
};