class BezierSegment : public EventListener
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

	Vector2 lerpQuadP(float t)
	{
		Vector2 linearA = lerp(controlPoints[0], controlPoints[1], t);
		Vector2 linearB = lerp(controlPoints[1], controlPoints[2], t);
		return lerp(linearA, linearB, t);
	}

	Vector2 lerpHepticP(float t)
	{
		Vector2 linearA = lerp(controlPoints[0], controlPoints[1], t);
		Vector2 linearB = lerp(controlPoints[1], controlPoints[2], t);
		Vector2 linearC = lerp(controlPoints[2], controlPoints[3], t);
		Vector2 linearD = lerp(controlPoints[4], controlPoints[5], t);

		Vector2 quadA = lerp(linearA, linearB, t);
		Vector2 quadB = lerp(linearB, linearC, t);
		Vector2 quadC = lerp(linearD, linearD, t);

		Vector2 cubicA = lerp(quadA, quadB, t);
		Vector2 cubicB = lerp(quadB, quadC, t);

		return lerp(cubicA, cubicB, t);
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
	BezierSegment(Vector2 A, Vector2 B) :
		m_nActivePoint(-1),
		m_fControlPointSize(0.5)
	{
		controlPoints.push_back(A);
		controlPoints.push_back(lerp(A, B, 0.3));
		controlPoints.push_back(lerp(A, B, 0.3));
		controlPoints.push_back(B);

		registerListener(MOUSEWORLD_EVENT);
	}

	vector<Vector2> getPoints()
	{
		return controlPoints;
	}

	void render(Render2D* renderer, Vector3 cameraPos)
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
		for (float t = 0; t < 1.0 - resolution; t = t + resolution)
		{
			Vector2 scaledPointA = cameraPos.toVec2() + lerpCubicP(t) * cameraPos.Z;

			renderer->DrawPoint(scaledPointA.X, scaledPointA.Y,  { PIXEL_SOLID, FG_LIGHTBLUE });
		}
		


		//	draw control points
		for (int i = 0; i < controlPoints.size(); i++)
		{
			Vector2 position = cameraPos.toVec2() + controlPoints[i] * cameraPos.Z;
			Vector2 text = cameraPos.toVec2() + (controlPoints[i] + Vector2(0, -0.2)) * cameraPos.Z;

			renderer->DrawCircle(position.X, position.Y, fRadius, { PIXEL_SOLID, FG_LIGHTRED });
			if (i == 0 || i == 3)
				renderer->DrawCircle(position.X, position.Y, fRadius, { PIXEL_SOLID, FG_YELLOW });
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

class BezierPath : public EventListener
{
private:
	vector<BezierSegment*> Path;

public:
	BezierPath()
	{
		Vector2 A = {3, 3};
		Vector2 B = {9, 9};
		Vector2 C = { 15, 15 };

		BezierSegment *bA = new BezierSegment(A, B);
		BezierSegment *bB = new BezierSegment(B, C);
		BezierSegment* bC = new BezierSegment(C, A);

		Path.push_back(bA);
		Path.push_back(bB);
		Path.push_back(bC);
	};

	~BezierPath()
	{
		for (auto b : Path)
		{
			delete b;
		}
	}

	void render(Render2D* renderer, Vector3 cameraPos)
	{
		int n = 0;
		float fRadius = 0.5 * cameraPos.Z;
		float resolution = 0.1 / cameraPos.Z;

		for (auto segment : Path)
		{
			for (int i = 0; i < segment->getPoints().size() - 1; i++)
		
			{
				//	draw lines between control points
				Vector2 A = cameraPos.toVec2() + segment->getPoints()[i] * cameraPos.Z;
				Vector2 B = cameraPos.toVec2() + segment->getPoints()[i + 1] * cameraPos.Z;
				renderer->DrawLine(A, B, { PIXEL_QUARTER, FG_WHITE });
			}
			//	draw control points
			for (int i = 0; i < segment->getPoints().size(); i++)
			{
				Vector2 position = cameraPos.toVec2() + segment->getPoints()[i] * cameraPos.Z;
				Vector2 text = cameraPos.toVec2() + (segment->getPoints()[i] + Vector2(0, -0.2)) * cameraPos.Z;

				renderer->DrawCircle(position.X, position.Y, fRadius, { PIXEL_SOLID, FG_LIGHTRED });
				if (i == 0 || i == 3)
					renderer->DrawCircle(position.X, position.Y, fRadius, { PIXEL_SOLID, FG_YELLOW });
				renderer->DrawString(thingToString<int>(n), text.X, text.Y - 2);
				if (i != 0)
					n++;
			}
			//	draw active point
			/*if (m_nActivePoint >= 0)
			{
				Vector2 position = cameraPos.toVec2() + controlPoints[m_nActivePoint] * cameraPos.Z;
				renderer->DrawCircle(position.X, position.Y, fRadius, { PIXEL_SOLID, FG_LIGHTGREEN });
			}*/
			//b->render(renderer, cameraPos);
		}
	}
};

