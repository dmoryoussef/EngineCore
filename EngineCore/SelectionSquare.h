class SelectionSquare : public EventListener
{
private:
	Vector2 vStart;
	Vector2 vStop;

	bool m_bReleased;
	bool m_bDrawQuad;
	bool m_bDrawLine;

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case MOUSEWORLD_EVENT:
			{
				MouseWorldEvent* pMouseEvent = pEvent->get<MouseWorldEvent>();
				m_bReleased = !pMouseEvent->getState().bLeftButtonDown;
				vStop = pMouseEvent->getWorldPosition();

				if (!m_bDrawQuad && m_bDrawLine)
					addEvent(new SelectionLineEvent(vStart, vStop, m_bReleased));

				if (m_bDrawQuad)
					addEvent(new SelectionSquareEvent(Min(), Max(), vStart, vStop, m_bReleased));

				break;
			}
		}
	}

	//	min/max as INT will give tile aligned square
	//	float will be smooth

	Vector2 Min()
	{
		float minY = vStart.Y;
		float minX = vStart.X;
		if (vStop.X < minX)
			minX = vStop.X;
		if (vStop.Y < minY)
			minY = vStop.Y;

		return Vector2(minX, minY);
	}

	Vector2 Max()
	{
		float maxY = vStop.Y;
		float maxX = vStop.X;
		if (vStart.X > maxX)
			maxX = vStart.X;
		if (vStart.Y > maxY)
			maxY = vStart.Y;
		return Vector2(maxX, maxY);
	}

public:
	SelectionSquare(Vector2 start, bool drawLine = true, bool drawQuad = true) :
		m_bReleased(false),
		vStart(start),
		vStop(start),
		m_bDrawQuad(drawQuad),
		m_bDrawLine(drawLine)
	{
		registerListener(MOUSEWORLD_EVENT);
		registerListener(KEYBOARD_EVENT);
	};

	bool isReleased()
	{
		return m_bReleased;
	}

	void render(Render2D* pRenderer)
	{
		//	draw:
		if (m_bDrawQuad)
			pRenderer->DrawQuad(Min().X, Min().Y, Max().X, Max().Y, {PIXEL_SOLID, FG_WHITE});
		//	or draw a line from start to stop
		if (m_bDrawLine)
			pRenderer->DrawLine(vStart, vStop, { PIXEL_SOLID, FG_WHITE });
		
		
		// show some data:
		vector<string> data;
		data.push_back(Min().toString());
		data.push_back(Max().toString());
		pRenderer->DrawString(data, Max().X + 1, Max().Y);
	}
};
