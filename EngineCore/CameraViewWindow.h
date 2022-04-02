class CameraViewWindow : public WorldViewWindow
{
private:
	BaseNode *m_pCamera;
	bool m_bCameraZoomable;
	float fMinZoom;
	float fMaxZoom;
	bool m_bCameraDragging;

	void handleWorldPosition(MouseState mouseState)
	{
		Vector3 vCamera = m_pCamera->getChild<Transform>()->getPosition();
		Vector2 worldPosition = WorldPosition(vCurrentMousePosition, vCamera.toVec2(), Position, vCamera.Z);
		addEvent(new MouseWorldEvent(worldPosition, mouseState));
	}

	void handleCameraMove(MouseState mouseState)
	{
		Vector3 vCurrentCameraPosition = m_pCamera->getChild<Transform>()->getPosition();
		// handle scaling
		if (m_bCameraZoomable)
		{
			if (mouseState.bWheeledDown)
			{
				if (vCurrentCameraPosition.Z > fMinZoom)
				{
					Vector2 vBeforeScaleWorldPosition = WorldPosition(vCurrentMousePosition, vCurrentCameraPosition.toVec2(), Position, vCurrentCameraPosition.Z);
					vCurrentCameraPosition.Z = vCurrentCameraPosition.Z * 0.85;

					Vector2 vAfterScaleScaleWorldPosition = WorldPosition(vCurrentMousePosition, vCurrentCameraPosition.toVec2(), Position, vCurrentCameraPosition.Z);
					Vector2 Diff = (vBeforeScaleWorldPosition - vAfterScaleScaleWorldPosition) * vCurrentCameraPosition.Z;

					m_pCamera->getChild<Transform>()->setPosition({ vCurrentCameraPosition.X - Diff.X, vCurrentCameraPosition.Y - Diff.Y, vCurrentCameraPosition.Z });
				}
			}

			if (mouseState.bWheeledUp)
			{
				if (vCurrentCameraPosition.Z < fMaxZoom)
				{
					Vector2 BeforeScaleWorldPosition = WorldPosition(vCurrentMousePosition, vCurrentCameraPosition.toVec2(), Position, vCurrentCameraPosition.Z);
					vCurrentCameraPosition.Z = vCurrentCameraPosition.Z * 1.15;

					Vector2 AfterScaleScaleWorldPosition = WorldPosition(vCurrentMousePosition, vCurrentCameraPosition.toVec2(), Position, vCurrentCameraPosition.Z);
					Vector2 Diff = (BeforeScaleWorldPosition - AfterScaleScaleWorldPosition) * vCurrentCameraPosition.Z;

					m_pCamera->getChild<Transform>()->setPosition({ vCurrentCameraPosition.X - Diff.X, vCurrentCameraPosition.Y - Diff.Y, vCurrentCameraPosition.Z });
				}
			}
		}

		// handle dragging
		if (!m_bDragActive)
		{
			{
				bool bMouseDown = mouseState.bWheelButtonDown;
				if (!m_bCameraDragging)
				{
					if (bMouseDown)
					{
						if (m_bMouseOver)
						{
							m_bCameraDragging = true;
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
							Vector3 vNewPosition = Vector3(vCurrentCameraPosition.X - vDeltaPos.X,
								vCurrentCameraPosition.Y - vDeltaPos.Y,
								vCurrentCameraPosition.Z);
							m_pCamera->getChild<Transform>()->setPosition(vNewPosition);
						}
					}
					else
						m_bCameraDragging = false;
				}
			}
		}
		else
		{
			if (m_bCameraDragging)
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
		Render2D* pRenderer = new Render2D(this);
		Vector3 vCurrentCameraPosition = m_pCamera->getChild<Transform>()->getPosition();
		Vector2 vWorldMin = WorldPosition({ 0, 0 }, vCurrentCameraPosition.toVec2(), Position, vCurrentCameraPosition.Z);
		Vector2 vWorldMax = WorldPosition(Size + Position, vCurrentCameraPosition.toVec2(), Position, vCurrentCameraPosition.Z);

		while (pBaseNode->isIterating())
		{
			pBaseNode->getCurrent()->render(pRenderer, vCurrentCameraPosition, vWorldMin, vWorldMax);
		}

	}

public:
	CameraViewWindow(int nWidth, int nHeight, int nPosX, int nPosY) :
		m_bCameraZoomable(false),
		fMinZoom(0.5),
		fMaxZoom(15.0),
	    m_bCameraDragging(false),
		WorldViewWindow(nWidth, nHeight, nPosX, nPosY)
	{
		m_pCamera = new BaseNode("Camera");
		m_pCamera->add(new Transform({0, 0, 5}, { 0, 0, 0 }, { 0, 0, 0 }));
	}

	BaseNode *getCamera()
	{
		return m_pCamera;
	}
};