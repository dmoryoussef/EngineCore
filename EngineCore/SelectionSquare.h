class SelectionSquare : public EventListener
{
private:
	Vector2 vStart;
	Vector2 vStop;

	bool m_bHovering;

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case MOUSEWORLD_EVENT:
			{
				MouseWorldEvent* pMouseEvent = pEvent->get<MouseWorldEvent>();

				vStop = pMouseEvent->getWorldPosition();

				addEvent(new SelectionSquareEvent(Min(), Max(), vStart, vStop, pMouseEvent->getState().bLeftButtonDown));

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
	SelectionSquare(Vector2 start) :
		m_bHovering(true),
		vStart(start),
		vStop(start)
	{
		registerListener(MOUSEWORLD_EVENT);
		registerListener(KEYBOARD_EVENT);
	};
	~SelectionSquare()
	{
		//	addEvent(new SelectionSquareEvent(Min(), Max(), vStart, vStop, m_bHovering));
	}

	bool isActive()
	{
		return m_bHovering;
	}

	void render(Render2D* pRenderer)
	{
		//	draw:
		pRenderer->DrawQuad(Min().X, Min().Y, Max().X, Max().Y, {PIXEL_SOLID, FG_WHITE});
		//	or draw a line from start to stop
		pRenderer->DrawLine(vStart, vStop, { PIXEL_SOLID, FG_WHITE });
		// show some data:
		pRenderer->DrawString(Min().toString(), Max().X + 1, Max().Y - 3);
		pRenderer->DrawString(Max().toString(), Max().X + 1, Max().Y - 2);
	}

	void render(Render2D* pRenderer, Vector3 vCamera)
	{
		// scale to screen:
		Vector2 vScaledMin = vCamera.toVec2() + Min() * vCamera.Z;
		Vector2 vScaledMax = vCamera.toVec2()  + Max() * vCamera.Z;  
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
		// draw bezier curve for rounded edges:
		//BezierSegment bezier(vStart, vStop);
		//bezier.setPoint(1, { Min().X, Max().Y });
		//bezier.setPoint(2, { Min().X, Max().Y });
		//bezier.render(pRenderer, vCamera);
	}

};
