class Vector2Demo : public GameState
{
private:
	Vector2 mouseP;
	Vector2 normal;
	Transform3D* pCameraTransform;

	void onEvent(_Event* pEvent)
	{
		if (MouseWorldEvent* pMouseEvent = pEvent->get<MouseWorldEvent>())
		{
			if (pMouseEvent->getState().bLeftButtonDown)
				mouseP = pMouseEvent->getWorldPosition();
			if (pMouseEvent->getState().bRightButtonDown)
				normal = pMouseEvent->getWorldPosition();
		}
	}

public:
	Vector2Demo() :
		mouseP(1, 1),
		normal(1, 0),
		pCameraTransform(NULL)
	{
		registerListener(MOUSEWORLD_EVENT);
	}


	void start(BaseNode* pData, BaseNode* pSystems, BaseNode* pGUI)
	{

		int height = pGUI->cast<_UIComponent>()->getHeight();
		int width = pGUI->cast<_UIComponent>()->getWidth();
		CameraViewWindow* pCameraWindow = new CameraViewWindow(width, height, 0, 0);
		BaseNode* pCamera = pCameraWindow->getCamera();
		pCameraTransform = pCamera->getChild<Transform3D>();
		pCameraTransform->setPosition({ 125, 75, 15 });	// screen space
		pData->add(pCamera);
		pGUI->addAtEnd(pCameraWindow);
	}


	void render(OutputBuffer* pEngineBuffer)
	{
		Render2D renderer(pEngineBuffer, pCameraTransform->getPosition());

		Vector2 a = Vector2(0, 0) - mouseP;
		Vector2 r = reflect(a, normal);

		renderer.DrawVector(a, mouseP, { PIXEL_SOLID, FG_LIGHTBLUE });
		renderer.DrawVector(normal, { 0, 0 }, { PIXEL_SOLID, FG_WHITE });
		renderer.DrawVector(r, { 0, 0 }, { PIXEL_SOLID, FG_LIGHTRED });
	}
};