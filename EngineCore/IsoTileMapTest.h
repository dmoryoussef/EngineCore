class IsoTileMapTest : public GameState
{
private:
	OrthographicTileMap tilemap;

public:

	void start(BaseNode* pData, BaseNode* pSystems, BaseNode* pGUI)
	{
		CameraViewWindow* pCameraWindow = new CameraViewWindow(100, 100, 0, 0);
		pData->add(pCameraWindow->getCamera());
		pGUI->addAtEnd(pCameraWindow);

		pData->add(&tilemap);
	}

	void update(BaseNode* pData, float fDeltaTime)
	{

	}

	void render(OutputBuffer* pEngineBuffer)
	{
	}
};