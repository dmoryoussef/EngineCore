class BezierCurve : public EventListener
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


	struct line
	{
		Vector2 a;
		Vector2 b;
	};

	
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
	BezierCurve(Vector2 A, Vector2 B) :
		m_nActivePoint(-1),
		m_fControlPointSize(0.5)
	{
		controlPoints.push_back(A);
		controlPoints.push_back(lerp(A, B, 0.3));
		controlPoints.push_back(lerp(A, B, 0.6));
		controlPoints.push_back(B);

		registerListener(MOUSEWORLD_EVENT);
	}

	int getActiveControlP()
	{
		return m_nActivePoint;
	}

	vector<Vector2> getPoints()
	{
		return controlPoints;
	}

	void setPoint(int i, Vector2 p)
	{
		controlPoints[i] = p;
	}

	void render(Render2D* renderer, float scale)
	{
		float fRadius = m_fControlPointSize;
		float resolution = 0.1 / scale;

		//	draw lines between control points
		for (int i = 0; i < controlPoints.size() - 1; i++)
		{
			Vector2 A = controlPoints[i];
			Vector2 B = controlPoints[i + 1];
			renderer->DrawLine(A, B, { PIXEL_QUARTER, FG_WHITE });
		}

		//	draw curve
		for (float t = 0; t < 1.0 - resolution; t = t + resolution)
		{
			Vector2 scaledPointA = lerpCubicP(t);
			Vector2 scaledPointB = lerpCubicP(t + resolution);
			renderer->DrawLine(scaledPointA, scaledPointB,  { PIXEL_SOLID, FG_LIGHTBLUE });
		}
		//	final segment
		Vector2 scaledPointA = lerpCubicP(1 - resolution);
		Vector2 scaledPointB = lerpCubicP(1);
		renderer->DrawLine(scaledPointA, scaledPointB, { PIXEL_SOLID, FG_LIGHTBLUE });

		
		//	draw control points
		for (int i = 0; i < controlPoints.size(); i++)
		{
			Vector2 position = controlPoints[i];

			renderer->DrawCircle(position.X, position.Y, fRadius, { PIXEL_SOLID, FG_LIGHTRED });
			if (i == 0 || i == 3)
				renderer->DrawCircle(position.X, position.Y, fRadius, { PIXEL_SOLID, FG_YELLOW });

			renderer->DrawString(position.toString(), position.X, position.Y + 1);

		}
		//	draw active point
		if (m_nActivePoint >= 0)
		{
			Vector2 position = controlPoints[m_nActivePoint];
			renderer->DrawCircle(position.X, position.Y, fRadius, { PIXEL_SOLID, FG_LIGHTGREEN });
		}
	}


	Vector2 cubicP(float t)
	{
		//	finds the point at t not using lerp
		Vector2 p0 = controlPoints[0] * pow((1 - t), 3);							//	(1 - t)^3
		Vector2 p1 = controlPoints[1] * 3 * pow((1 - t), 2) * t;					//	3(1 - t)^2 * t
		Vector2 p2 = controlPoints[2] * 3 * (1 - t) * pow(t, 2);					//	3(1 - t)^2
		Vector2 p3 = controlPoints[3] * pow(t, 3);									//	t^3
		return p0 + p1 + p2 + p3;
	}

	Vector2 tangentP(float t)
	{
		float u = t - 1;
		//	derivative of the cubic formula to find the tangent (rate of change)
		float d0 = (-3 * u * u);					//	-3(1-t)^2
		float d1 = 3 * (t - 1) * ((3 * t) - 1);				//	3(t - 1) * (3t - 1)
		float d2 = 6 * (t - 1);								//	6(t - 1)
		float d3 = (3 * t * t);									//	3t^2

		Vector2 p0 = controlPoints[0] * d0;
		Vector2 p1 = controlPoints[1] * d1;
		Vector2 p2 = controlPoints[2] * d2;
		Vector2 p3 = controlPoints[3] * d3;
		Vector2 out = p0 + p1 + p2 + p3;
		return out;
	}

	line getLerpTan(float t)
	{
		Vector2 linearA = lerp(controlPoints[0], controlPoints[1], t);
		Vector2 linearB = lerp(controlPoints[1], controlPoints[2], t);
		Vector2 linearC = lerp(controlPoints[2], controlPoints[3], t);
		Vector2 quadA = lerp(linearA, linearB, t);
		Vector2 quadB = lerp(linearB, linearC, t);

		return { quadA, quadB };
	}

};

class BezierSpline : public EventListener
{
private:
	vector<BezierCurve*> Segments;
	vector<Vector2> Path;

	bool m_bMirroredState; // broken, mirrored, aligned

	void onKeyboardEvent(KeyboardEvent* pEvent)
	{
		if (pEvent->isKeyDown('m'))
			m_bMirroredState = true;
		if (pEvent->isKeyUp('m'))
			m_bMirroredState = false;
	}

	void onMouseWorldEvent(MouseWorldEvent* pEvent)
	{
		MouseState mouseState = pEvent->getState();
		Vector2 worldPosition = pEvent->getWorldPosition();

		//	mirroreda
		if (m_bMirroredState)
		for (int i = 0; i < Segments.size(); i++)
		{
			if (mouseState.bLeftButtonDown)
			{
				if (Segments[i]->getActiveControlP() == 3)
				{
					//	move neighboring control points
					//		current segment->point 2
					//		next segment->point 0
				}

				if (Segments[i]->getActiveControlP() == 0)
				{
					//	move neighboring control points
					//		current segment -> point 1
					//		prev segment -> point 3
				}


				if (Segments[i]->getActiveControlP() == 1)
				{
					Vector2 mirror = Segments[i]->getPoints()[1].mirror(Segments[i]->getPoints()[0]);
					int prev = (i - 1) % Segments.size();
					Segments[prev]->setPoint(2, mirror);
				}
				if (Segments[i]->getActiveControlP() == 2)
				{
					Vector2 mirror = Segments[i]->getPoints()[2].mirror(Segments[i]->getPoints()[3]);
					int next = (i + 1) % Segments.size();
					Segments[next]->setPoint(1, mirror);
				}
			}
		}
	}

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case MOUSEWORLD_EVENT: onMouseWorldEvent(pEvent->get<MouseWorldEvent>());
				break;
			case KEYBOARD_EVENT: onKeyboardEvent(pEvent->get<KeyboardEvent>());
				break;
		}
		
	}

public:
	BezierSpline() 
	{
		registerListener(MOUSEWORLD_EVENT);
		registerListener(KEYBOARD_EVENT);
	};

	~BezierSpline()
	{
		for (auto b : Segments)
		{
			delete b;
		}
	}

	vector<BezierCurve*> segments()
	{
		return Segments;
	}

	BezierCurve *getSegment(int i)
	{
		return Segments[i];
	}

	void makePoints(float resolution)
	{
		Path.clear();
		for (auto segment : Segments)
		{
			for (float t = 0; t < 1.0 - resolution; t = t + resolution)
			{
				Path.push_back(segment->cubicP(t));
			}
		}
	}

	/*Vector2 lerpTanP(int n, float t)
	{
		BezierCurve* segment = Segments[n];
		return segment->getLerpTan(t);
	}*/

	Vector2 cubicP(float u)
	{
		int n = (int)u;
		float t = u - (float)n;
		BezierCurve *segment = Segments[n % (Segments.size() - 1)];
		return segment->cubicP(t);
	}



	void addSegment(Vector2 A, Vector2 B)
	{
		BezierCurve *bs = new BezierCurve(A, B);
		Segments.push_back(bs);
	}

	void render(Render2D* renderer, float scale)
	{
		for (auto seg : Segments)
			seg->render(renderer, scale);

		/*int n = 0;
		for (int i = 0; i < Segments.size(); i++)
		{
			for (int j = 0; j < Segments[i]->getPoints().size() - 1; j++)
			{
				Vector2 text = cameraPos.toVec2() + (Segments[i]->getPoints()[j] + Vector2(0, -0.2)) * cameraPos.Z;
				renderer->DrawString(thingToString<int>(n), text.X, text.Y - 2);
				n++;
			}
		}*/
	
		/*makePoints(0.1 / cameraPos.Z);
		for (int i = 0; i < Path.size() - 1; i++)
		{
			Vector2 scaledPointA = cameraPos.toVec2() + Path[i] * cameraPos.Z;
			Vector2 scaledPointB = cameraPos.toVec2() + Path[i + 1] * cameraPos.Z;
			renderer->DrawLine(scaledPointA, scaledPointB, { PIXEL_SOLID, FG_LIGHTBLUE });
		}*/


		////	render tangents....very broken
		//for (auto s : Segments)
		//{
		//	Vector2 T = s->tangentP(0.5);
		//	Vector2 P = s->cubicP(0.5);
		//	Vector2 normal = getNormal(P, T);
		//	normal = normal.normalize();

		//	Vector2 scaledT = cameraPos.toVec2() + T * cameraPos.Z;
		//	Vector2 scaledP = cameraPos.toVec2() + P * cameraPos.Z;
		//	Vector2 scaledN = cameraPos.toVec2() + normal * cameraPos.Z;
		//	renderer->DrawLine(scaledP, scaledT, {PIXEL_SOLID, FG_LIGHTBLUE});
		//}
	}
};

