class Win32Demo : public GameState
{
private:
	Transform3D* pCameraTransform;

public:
	Win32Demo() {}

	void start(BaseNode* pData, BaseNode* pSystems, BaseNode* pGUI)
	{
		cout << "test to console window.";
	}

	void update(BaseNode* pData, float fDeltaTime) {}

	void render(OutputBuffer* pEngineBuffer)
	{
		//	Render2D renderer(pEngineBuffer, pCameraTransform->getPosition());
	}
};