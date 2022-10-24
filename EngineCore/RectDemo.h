class Rect
{
private:
	Vector2 vMin;
	Vector2 vMax;

public:
	Rect(Vector2 min, Vector2 max) :
		vMin(min),
		vMax(max) {};

	Vector2 getSize()
	{
		return vMax - vMin;
	}

	Vector2 getMin()
	{
		return vMin;
	}

	Vector2 getMax()
	{
		return vMax;
	}

	float getPerimeter()
	{
		return 0;
	}

	float getArea()
	{
		return 0;
	}
};

class RectDemo : public GameState
{
private:
	Transform3D* pCameraTransform;
	EditablePoly2D* poly;
	PolyList Polys;

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

		Rect A(Polys.get(0)->getMin(), Polys.get(0)->getMax());
		Rect B(Polys.get(1)->getMin(), Polys.get(1)->getMax());

		Vector2 min = Vector2(max(A.getMin().X, B.getMin().X), max(A.getMin().Y, B.getMin().Y));
		Vector2 max = Vector2(min(A.getMax().X, B.getMax().X), min(A.getMax().Y, B.getMax().Y));

		Rect C(min, max);
		renderer.DrawQuad(C.getMin().X, C.getMin().Y, C.getMax().X, C.getMax().Y, { PIXEL_SOLID, FG_LIGHTRED });
	}
};