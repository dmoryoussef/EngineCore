class RogueLikeDemo : public GameState
{
private:
	BaseNode *Player;

public:
	RogueLikeDemo() :
		Player(NULL) {};

	void start(BaseNode* pData, BaseNode* pSystems, BaseNode* pGUI)
	{
		int height = pGUI->cast<_UIComponent>()->getHeight();
		int width = pGUI->cast<_UIComponent>()->getWidth();
		CameraViewWindow* pCameraWindow = new CameraViewWindow(width, height, 0, 0);
		pData->add(pCameraWindow->getCamera());
		pGUI->addAtEnd(pCameraWindow);

		Player = new BaseNode();
		Player->addChild(new Render(3));
		Player->addChild(new Transform2D({ float(random(1, 10)), float(random(1, 10)) }, { 0, 0 }, { 0.5, 0.5 }));
		Player->addChild(new Velocity());
		Player->addChild(new UserController(0, DIRECT));

		pData->add(Player);
		pData->add(new DefaultTileMap(100, 100));
	}
};