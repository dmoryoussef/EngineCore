class BezierDemo : public GameState
{
private:
	BezierPath bezier;
	Transform3D* pCamera;

public:
	BezierDemo() :
		bezier()
	{
		Vector2 A = { 3, 3 };
		Vector2 B = { 22, 3 };
		Vector2 C = { 11, 15 };

		bezier.addSegment(A, B);
		bezier.addSegment(B, C);
		bezier.addSegment(C, A);

		registerListener(MOUSEWORLD_EVENT);
		registerListener(KEYBOARD_EVENT);
		registerListener(GUI_EVENT);
	};


	void start(BaseNode* pData, BaseNode* pSystems, BaseNode* pGUI)
	{
		CameraViewWindow* pCameraWindow = new CameraViewWindow(150, 100, 0, 0);
		pData->add(pCameraWindow->getCamera());
		pCamera = pCameraWindow->getCamera()->getChild<Transform3D>();
		pGUI->addChild(pCameraWindow);
	}

	void render(OutputBuffer* pEngineBuffer)
	{
		//	currently renders outside "window" becuase there is no bounding information
		//	move to window to fix??
		Render2D renderer(pEngineBuffer);
		//	bezier.render(&renderer, pCamera->getPosition());
	}

};