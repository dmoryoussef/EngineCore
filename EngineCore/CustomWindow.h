class CustomWindow : public CameraViewWindow
{
private:
	void onEvent(_Event* pEvent)
	{
		_UIComponent::onEvent(pEvent);
		switch (pEvent->m_eType)
		{
			case CONSOLE_MOUSE_EVENT:
			{
				MouseState state = pEvent->get<MouseEvent>()->getState();
				
				for (int i = 0; i < spline.size(); i++)
				{
					spline[i].checkMouse(state.Position, state.bLeftButtonDown);
				}
				
				break;
			}
		}
	}

public:
	CustomWindow(int nWidth, int nHeight, int nPosX, int nPosY) :
		CameraViewWindow(nWidth, nHeight, nPosX, nPosY) {};

	struct splineSegment
	{
		vector<Vector2> controlPoints;
		vector<Vector2> lerpedA;
		vector<Vector2> lerpedB;
		vector<Vector2> lerpedC;
		vector<Vector2> quadraticA;
		vector<Vector2> quadraticB;
		vector<Vector2> cubic;

		float resolution;
		int activeControlPoint;

		splineSegment(Vector2 A, Vector2 B)
		{
			activeControlPoint = -1;
			resolution = 0.05;

			controlPoints.push_back(A);
			controlPoints.push_back(lerp(A, B, 0.2));
			controlPoints.push_back(lerp(A, B, 0.8));
			controlPoints.push_back(B);
		}

		void updateLerped()
		{
			lerpedA.clear();
			lerpedB.clear();
			lerpedC.clear();

			for (float t = 0; t < 1.01; t = t + resolution)
			{
				lerpedA.push_back(lerp(controlPoints[0], controlPoints[1], t));
				lerpedB.push_back(lerp(controlPoints[1], controlPoints[2], t));
				lerpedC.push_back(lerp(controlPoints[2], controlPoints[3], t));
			}
		}

		void updateQuadratic()
		{
			quadraticA.clear();
			quadraticB.clear();

			int i = 0;
			for (float t = 0; t < 1.01; t = t + resolution)
			{
				quadraticA.push_back(lerp(lerpedA[i], lerpedB[i], t));
				quadraticB.push_back(lerp(lerpedB[i], lerpedC[i], t));
				i++;
			}
		}

		void updateCubic()
		{
			cubic.clear();
			int i = 0;
			for (float t = 0; t < 1.01; t = t + resolution)
			{
				cubic.push_back(lerp(quadraticA[i], quadraticB[i], t));
				i++;
			}
		}

		Vector2 lerpedCubicPoint(float t)
		{
			Vector2 linearA = lerp(controlPoints[0], controlPoints[1], t);
			Vector2 linearB = lerp(controlPoints[1], controlPoints[2], t);
			Vector2 linearC = lerp(controlPoints[2], controlPoints[3], t);
			Vector2 quadA = lerp(linearA, linearB, t);
			Vector2 quadB = lerp(linearB, linearC, t);
			Vector2 cubic = lerp(quadA, quadB, t);
			return cubic;
		}

		Vector2 cubicBezierP(float t)
		{
			Vector2 p0 = controlPoints[0] * pow((1 - t), 3);
			Vector2 p1 = controlPoints[1] * 3 * pow((1 - t), 2) * t;
			Vector2 p2 = controlPoints[2] * 3 * (1 - t) * pow(t, 2);
			Vector2 p3 = controlPoints[3] * pow(t, 3);
			return p0 + p1 + p2 + p3;
		}

		void checkMouse(Vector2 mousePos, bool bPressed)
		{
			for (int i = 0; i < controlPoints.size(); i++)
				if (mousePos == controlPoints[i])
				{
					if (bPressed)
						activeControlPoint = i;
					else
						activeControlPoint = -1;
				}

			if (activeControlPoint >= 0)
				controlPoints[activeControlPoint] = mousePos;
		}

	};

	vector<splineSegment> spline;

	void start()
	{
		spline.push_back(splineSegment({10, 10}, {40, 10}));
		spline.push_back(splineSegment({ 40, 10 }, { 60, 10 }));
	}


	void constructComponent(BaseNode* pBaseNode)
	{
		Render2D render2d(this);
		//	to enable scroll and zoom: need to pass camera data to renderer or something

		/*for (auto p : segment.lerpedA)
		{
			render2d.DrawCircle(p.X, p.Y, .5, { PIXEL_QUARTER, FG_LIGHTRED });
		}
		for (auto p : segment.lerpedB)
		{
			render2d.DrawCircle(p.X, p.Y, .5, { PIXEL_QUARTER, FG_LIGHTRED });
		}
		for (auto p : segment.lerpedC)
		{
			render2d.DrawCircle(p.X, p.Y, .5, { PIXEL_QUARTER, FG_LIGHTRED });
		}

		for (auto p : segment.quadraticA)
		{
			render2d.DrawCircle(p.X, p.Y, .5, { PIXEL_QUARTER, FG_WHITE });
		}

		for (auto p : segment.quadraticB)
		{
			render2d.DrawCircle(p.X, p.Y, .5, { PIXEL_QUARTER, FG_WHITE });
		}

		for (int i = 0; i < segment.cubic.size() - 1; i++)
		{
			render2d.DrawLine(segment.cubic[i], segment.cubic[i + 1], {PIXEL_SOLID, FG_YELLOW});
		}*/

		/*for (float t = 0; t < 1.001; t = t + 0.05)
		{
			Vector2 p = segment.cubicP(t);
			render2d.DrawCircle(p.X, p.Y, .5, {PIXEL_QUARTER, FG_WHITE});
		}*/

		for (int i = 0; i < spline.size(); i++)
		{
			for (float t = 0; t < 1.001; t = t + 0.05)
			{
				Vector2 p = spline[i].cubicBezierP(t);
				render2d.DrawCircle(p.X, p.Y, .5, { PIXEL_QUARTER, FG_YELLOW });
			}

			for (auto p : spline[i].controlPoints)
			{
				render2d.DrawCircle(p.X, p.Y, 1, { PIXEL_SOLID, FG_LIGHTBLUE });
			}
		}
		

		/*Vector2 cubicPrev = points[0];
		for (float t = 0; t < 1.01; t = t + 0.05)
		{
			Vector2 lerpedA = lerp(points[0], points[1], t);
			Vector2 lerpedB = lerp(points[1], points[2], t);
			Vector2 lerpedC = lerp(points[2], points[3], t);

			render2d.DrawCircle(lerpedA.X, lerpedA.Y, .5, { PIXEL_SOLID, FG_LIGHTRED });
			render2d.DrawCircle(lerpedB.X, lerpedB.Y, .5, { PIXEL_SOLID, FG_LIGHTRED });
			render2d.DrawCircle(lerpedC.X, lerpedC.Y, .5, { PIXEL_SOLID, FG_LIGHTRED });

			Vector2 quadraticA = lerp(lerpedA, lerpedB, t);
			Vector2 quadraticB = lerp(lerpedB, lerpedC, t);

			render2d.DrawCircle(quadraticA.X, quadraticA.Y, .5, { PIXEL_SOLID, FG_WHITE });
			render2d.DrawCircle(quadraticB.X, quadraticB.Y, .5, { PIXEL_SOLID, FG_WHITE });

			Vector2 cubic = lerp(quadraticA, quadraticB, t);
			render2d.DrawLine(cubic, cubicPrev, { PIXEL_SOLID, FG_YELLOW });
			cubicPrev = cubic;
		}

		for (auto p : points)
		{
			render2d.DrawCircle(p.X, p.Y, 1, {PIXEL_SOLID, FG_LIGHTBLUE});
		}*/



	}
	
	void update(float fDeltaTime)
	{
		//segment.updateLerped();
		//segment.updateQuadratic();
		//segment.updateCubic();
	}
};