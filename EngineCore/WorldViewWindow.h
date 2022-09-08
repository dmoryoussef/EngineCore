class WorldViewWindow : public UIWindow
{
protected:
	float m_fScreenScale;

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

	void handleWorldPosition(MouseState mouseState)
	{
		Vector2 worldPosition = WorldPosition(vCurrentMousePosition, Vector2(0, 0), Position, m_fScreenScale);
		addEvent(new MouseWorldEvent(worldPosition, mouseState));
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
		Render2D renderer(this);
		EntityRenderSystem ecsRenderer;
		ecsRenderer.render(pBaseNode, &renderer, Vector2(0, 0), Vector2(getWidth(), getHeight()));
		

		set("Mouse: " + vCurrentMousePosition.toString(), getWidth() - 20, 2, FG_WHITE);
		set("World: " + WorldPosition(vCurrentMousePosition, Vector2(0, 0), Position, m_fScreenScale).toString(), getWidth() - 20, 3, FG_WHITE);

	}

public:
	WorldViewWindow(int nWidth, int nHeight, int nPosX, int nPosY) :
		m_fScreenScale(1),
		UIWindow(nWidth, nHeight, nPosX, nPosY)
	{
		//registerListener(BASENODE_EVENT);
	}
};