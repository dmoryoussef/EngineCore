class BezierDemo : public GameState
{
private:
	BezierSpline bezier;
	Transform3D* pCamera;

	float u;

	int mouseX;

	void onMouseWorldEvent(MouseWorldEvent* pEvent)
	{
		mouseX = pEvent->getWorldPosition().X;
	}

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case MOUSEWORLD_EVENT: onMouseWorldEvent(pEvent->get<MouseWorldEvent>());
				break;
		}
	}
	Vector2 A = { 3, 3 };
	Vector2 B = { 22, 3 };

public:
	BezierDemo() :
		u(0),
		pCamera(NULL),
		bezier()
	{
		
		Vector2 C = { 11, 15 };

		bezier.addSegment(A, B);
		bezier.addSegment(B, C);
		// bezier.addSegment(C, A);

		registerListener(MOUSEWORLD_EVENT);
		registerListener(KEYBOARD_EVENT);
		registerListener(GUI_EVENT);
	};


	void start(BaseNode* pData, BaseNode* pSystems, BaseNode* pGUI)
	{
		CameraViewWindow* pCameraWindow = new CameraViewWindow(200, 150, 0, 0);
		pData->add(pCameraWindow->getCamera());
		pCamera = pCameraWindow->getCamera()->getChild<Transform3D>();
		pGUI->addChild(pCameraWindow);
	}

	void update(BaseNode* pData, float fDeltaTime)
	{
		if (u < (float)bezier.segments().size() - 0.001)
			u = u + .001;// *fDeltaTime;
		else
			u = 0;

	}


	void render(OutputBuffer* pEngineBuffer)
	{
		Render2D renderer(pEngineBuffer, pCamera->getPosition());
		bezier.render(&renderer, pCamera->getPosition().Z);
		Vector3 vCamPos = pCamera->getPosition();

		//float minX = bezier.getSegment(0)->getPoints()[0].X;
		//float maxX = bezier.getSegment(0)->getPoints()[3].X;
		//float t = (((float)mouseX - minX) / (maxX - minX));	// normalized
		//if (t < 0) t = 0;
		//if (t > 1) t = 1;
		
		renderer.DrawNum<int>(u, 2, 2, FG_WHITE);

		float t = u - (int)u;

		Vector2 pos = bezier.getSegment(u)->cubicP(t);
		Vector2 tan = (bezier.getSegment(u)->getLerpTan(t).b - pos).normalize();
		Vector2 right = tan.right();
		Vector2 left = tan.left();

		tan.mult(2);
		right.mult(2);
		left.mult(2);

		renderer.DrawCircle(pos.X, pos.Y, 0.5, { PIXEL_SOLID, FG_WHITE });
		renderer.DrawString(pos.toString(), pos.X, pos.Y + 1);

		renderer.DrawVector(tan, pos);
		renderer.DrawVector(right, pos);
		renderer.DrawVector(left, pos);
		
	}

};