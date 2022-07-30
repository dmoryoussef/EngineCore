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
				
				addEvent(new SelectionSquareEvent(Min(), Max(), vStart, vStop, m_bActive));

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
		m_bActive(true),
		vStart(start),
		vStop(start)
	{
		registerListener(MOUSEWORLD_EVENT);
		registerListener(KEYBOARD_EVENT);
	};
	~SelectionSquare()
	{
		addEvent(new SelectionSquareEvent(Min(), Max(), vStart, vStop, m_bActive));
	}

	bool isActive()
	{
		return m_bActive;
	}

	void render(Render2D* pRenderer, Vector3 vCamera)
	{
		// scale to screen:
		Vector2 vScaledMin = vCamera.toVec2() + Min() * vCamera.Z;
		Vector2 vScaledMax = vCamera.toVec2()  + Max() * vCamera.Z;  //	adding 1, 1 - should actually be *tile size*, only works in this form when tilesize = 1, 1
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

class Bezier : public EventListener
{
private:
	vector<Vector2> controlPoints;
	int m_nActivePoint;
	float m_fControlPointSize;

	Vector2 lerpQuadP(Vector2 a, Vector2 b, Vector2 c, int t)
	{
		Vector2 linearA = lerp(a, b, t);
		Vector2 linearB = lerp(b, c, t);
		return lerp(linearA, linearB, t);
	}

	Vector2 cubicP(float t)
	{
		//	finds the point at t not using lerp
		Vector2 p0 = controlPoints[0] * pow((1 - t), 3);
		Vector2 p1 = controlPoints[1] * 3 * pow((1 - t), 2) * t;
		Vector2 p2 = controlPoints[2] * 3 * (1 - t) * pow(t, 2);
		Vector2 p3 = controlPoints[3] * pow(t, 3);
		return p0 + p1 + p2 + p3;
	}

	Vector2 lerpQuadP(float t, int i)
	{
		Vector2 linearA = lerp(controlPoints[i], controlPoints[i + 1], t);
		Vector2 linearB = lerp(controlPoints[i + 1], controlPoints[i + 2], t);
		return lerp(linearA, linearB, t);
	}

	Vector2 lerpQuadP(float t)
	{
		Vector2 linearA = lerp(controlPoints[0], controlPoints[1], t);
		Vector2 linearB = lerp(controlPoints[1], controlPoints[2], t);
		return lerp(linearA, linearB, t);
	}


	Vector2 lerpCubicP(float t)
	{
		//	finds the point at t using the lerp function
		Vector2 linearA = lerp(controlPoints[0], controlPoints[1], t);
		Vector2 linearB = lerp(controlPoints[1], controlPoints[2], t);
		Vector2 linearC = lerp(controlPoints[2], controlPoints[3], t);
		Vector2 quadA = lerp(linearA, linearB, t);
		Vector2 quadB = lerp(linearB, linearC, t);
		Vector2 cubic = lerp(quadA, quadB, t);
		return cubic;
	}

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case MOUSEWORLD_EVENT:
			{
				MouseWorldEvent* pMouseEvent = pEvent->get<MouseWorldEvent>();
				MouseState mouseState = pMouseEvent->getState();
				Vector2 worldPosition = pMouseEvent->getWorldPosition();

				if (m_nActivePoint != -1 && pMouseEvent->getState().bLeftButtonDown)
				{
					controlPoints[m_nActivePoint] = worldPosition;
				}
				else
				{
					m_nActivePoint = -1;
					for (int i = 0; i < controlPoints.size(); i++)
					{
						if (hypotenuse(controlPoints[i], worldPosition) <= m_fControlPointSize)
						{
							m_nActivePoint = i;
						}
					}
				}
				break;
			}
		}
	}

public:
	Bezier(Vector2 A, Vector2 B) :
		m_nActivePoint(-1),
		m_fControlPointSize(0.5)
	{
		Vector2 C = { 12, 12 };
		Vector2 D = { 15, 15 };

		controlPoints.push_back(A);
		controlPoints.push_back({B.X, A.Y});
		controlPoints.push_back({A.X, B.Y});
		controlPoints.push_back(B);
		controlPoints.push_back(C);
		controlPoints.push_back(D);

		registerListener(MOUSEWORLD_EVENT);
	}

	void render(Render2D *renderer, Vector3 cameraPos)
	{
		float fRadius = m_fControlPointSize * cameraPos.Z;
		float resolution = 0.1 / cameraPos.Z;

		//	draw lines between control points
		for (int i = 0; i < controlPoints.size() - 1; i++)
		{
			Vector2 A = cameraPos.toVec2() + controlPoints[i] * cameraPos.Z;
			Vector2 B = cameraPos.toVec2() + controlPoints[i + 1] * cameraPos.Z;
			renderer->DrawLine(A, B, { PIXEL_QUARTER, FG_WHITE });
		}
		
		//	draw curve
		for (int i = 0; i < controlPoints.size() - 2; i = i + 2)
		{
			for (float j = 1; j <= 3; j++)
			{
				float t = j / 3.0f;
				Vector2 scaledPointA = cameraPos.toVec2() + lerpQuadP(controlPoints[i], controlPoints[i + 1], controlPoints[i + 2], t) * cameraPos.Z;

				renderer->DrawCircle(scaledPointA.X, scaledPointA.Y, 1.0, { PIXEL_SOLID, FG_WHITE });
			}
		}


		//	draw control points
		for (int i = 0; i < controlPoints.size(); i++)
		{
			Vector2 position = cameraPos.toVec2() + controlPoints[i] * cameraPos.Z;
			Vector2 text = cameraPos.toVec2() + (controlPoints[i] + Vector2(0, -0.2)) * cameraPos.Z;
			renderer->DrawCircle(position.X, position.Y, fRadius, { PIXEL_SOLID, FG_LIGHTRED });
			renderer->DrawString(thingToString<int>(i), text.X, text.Y - 2);
		}
		//	draw active point
		if (m_nActivePoint >= 0)
		{
			Vector2 position = cameraPos.toVec2() + controlPoints[m_nActivePoint] * cameraPos.Z;
			renderer->DrawCircle(position.X, position.Y, fRadius, { PIXEL_SOLID, FG_LIGHTGREEN });
		}
	}
};