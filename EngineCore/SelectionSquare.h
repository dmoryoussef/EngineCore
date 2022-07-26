class SelectionSquare : public EventListener
{
private:
	Vector2 vStart;
	Vector2 vStop;

	bool m_bActive;
	char m_chKey;

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case KEYBOARD_EVENT:
			{
				/*KeyboardEvent* pKeyboardEvent = pEvent->get<KeyboardEvent>();
				if (pKeyboardEvent->isKeyUp(m_chKey))
				{
					m_bActive = false;
				}*/
				break;
			}
			case MOUSEWORLD_EVENT:
			{
				MouseWorldEvent* pMouseEvent = pEvent->get<MouseWorldEvent>();
				if (!pMouseEvent->getState().bLeftButtonDown)
				{
					m_bActive = false;

				}
				
				vStop = pMouseEvent->getWorldPosition() + Vector2(1, 1);
				
				addEvent(new SelectionSquareEvent(Min(), Max(), m_bActive));

				break;
			}
		}
	}

	Vector2 Min()
	{
		int minY = vStart.Y;
		int minX = vStart.X;
		if (vStop.X < minX)
			minX = vStop.X;
		if (vStop.Y < minY)
			minY = vStop.Y;

		return Vector2(minX, minY);
	}

	Vector2 Max()
	{
		int maxY = vStop.Y;
		int maxX = vStop.X;
		if (vStart.X > maxX)
			maxX = vStart.X;
		if (vStart.Y > maxY)
			maxY = vStart.Y;
		return Vector2(maxX, maxY);
	}

public:
	SelectionSquare(Vector2 start) :
		m_chKey('s'),
		m_bActive(true),
		vStart(start),
		vStop(start)
	{
		registerListener(CONSOLE_MOUSE_EVENT);
		registerListener(MOUSEWORLD_EVENT);
		registerListener(KEYBOARD_EVENT);
	};
	~SelectionSquare()
	{
		addEvent(new SelectionSquareEvent(Min(), Max(), m_bActive));
	}

	bool isActive()
	{
		return m_bActive;
	}

	void render(Render2D* pRenderer, Vector3 vCamera)
	{
		// scale to screen:
		Vector2 vScaledMin = vCamera.toVec2() + Min() * vCamera.Z;
		Vector2 vScaledMax = vCamera.toVec2() + Max() * vCamera.Z;
		// draw:
		pRenderer->DrawQuad(vScaledMin.X, vScaledMin.Y, vScaledMax.X, vScaledMax.Y, { PIXEL_SOLID, FG_WHITE });
		// show some data:
		pRenderer->DrawString(Min().toString(), vScaledMax.X + 1, vScaledMax.Y - 3);
		pRenderer->DrawString(Max().toString(), vScaledMax.X + 1, vScaledMax.Y - 2);
	}

};