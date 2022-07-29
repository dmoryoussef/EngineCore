class SelectionSquare : public EventListener
{
private:
	Vector2 vStart;
	Vector2 vStop;

	bool m_bActive;

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case MOUSEWORLD_EVENT:
			{
				MouseWorldEvent* pMouseEvent = pEvent->get<MouseWorldEvent>();
				if (!pMouseEvent->getState().bLeftButtonDown)
				{
					m_bActive = false;
				}
				
				vStop = pMouseEvent->getWorldPosition();
				
				addEvent(new SelectionSquareEvent(Min(), (Max() + Vector2(1, 1)), vStart, vStop, m_bActive));

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
		m_bActive(true),
		vStart(start),
		vStop(start)
	{
		registerListener(MOUSEWORLD_EVENT);
		registerListener(KEYBOARD_EVENT);
	};
	~SelectionSquare()
	{
		addEvent(new SelectionSquareEvent(Min(), (Max() + Vector2(1, 1)), vStart, vStop, m_bActive));
	}

	bool isActive()
	{
		return m_bActive;
	}

	void render(Render2D* pRenderer, Vector3 vCamera)
	{
		// scale to screen:
		Vector2 vScaledMin = vCamera.toVec2() + Min() * vCamera.Z;
		Vector2 vScaledMax = vCamera.toVec2()  + (Max() + Vector2(1, 1)) * vCamera.Z;  //	adding 1, 1 - should actually be *tile size*, only works in this form when tilesize = 1, 1
		Vector2 vScaledStart = vCamera.toVec2() + vStart * vCamera.Z;
		Vector2 vScaledStop = vCamera.toVec2() + vStop * vCamera.Z;
		//	draw:
		pRenderer->DrawQuad(vScaledMin.X, vScaledMin.Y, vScaledMax.X, vScaledMax.Y, { PIXEL_SOLID, FG_WHITE });
		//	or draw a line from start to stop
		pRenderer->DrawLine(vScaledStart, vScaledStop, { PIXEL_SOLID, FG_WHITE });
		// show some data:
		pRenderer->DrawString(Min().toString(), vScaledMax.X + 1, vScaledMax.Y - 3);
		pRenderer->DrawString(Max().toString(), vScaledMax.X + 1, vScaledMax.Y - 2);

		//pRenderer->DrawQuad(vScaledStart.X, vScaledStart.Y, vScaledStart.X, vScaledStart.Y, { PIXEL_SOLID, FG_LIGHTBLUE });
	}

};