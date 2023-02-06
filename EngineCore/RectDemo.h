class RectDemo : public GameState
{
private:
	Transform3D* pCameraTransform;
	EditablePoly2D* poly;
	PolyList<EditablePoly2D> Polys;

public:
	RectDemo() {};

	void start(BaseNode* pData, BaseNode* pSystems, BaseNode* pGUI)
	{
		int height = pGUI->cast<_UIComponent>()->getHeight();
		int width = pGUI->cast<_UIComponent>()->getWidth();
		CameraViewWindow* pCameraWindow = new CameraViewWindow(width, height, 0, 0);
		BaseNode* pCamera = pCameraWindow->getCamera();
		pCameraTransform = pCamera->getChild<Transform3D>();
		pData->add(pCamera);
		pGUI->addAtEnd(pCameraWindow);

		Polys.addNewPoly(new EditablePoly2D({ 15, 15 }, { 20, 20 }));
		Polys.addNewPoly(new EditablePoly2D({ 5, 5 }, { 15, 15 }));
	}

	void update(BaseNode* pData, float fDeltaTime)
	{
	}

	void render(OutputBuffer* pEngineBuffer)
	{
		Render2D renderer(pEngineBuffer, pCameraTransform->getPosition());

		Polys.render(&renderer);

		Rect2D A(Polys.get(0)->getMin(), Polys.get(0)->getMax());
		Rect2D B(Polys.get(1)->getMin(), Polys.get(1)->getMax());

		Vector2 min = Vector2(max(A.getMin().X, B.getMin().X), max(A.getMin().Y, B.getMin().Y));
		Vector2 max = Vector2(min(A.getMax().X, B.getMax().X), min(A.getMax().Y, B.getMax().Y));

		Rect2D C(min, max);
		C.render(&renderer, { PIXEL_SOLID, FG_LIGHTRED });
	}
};