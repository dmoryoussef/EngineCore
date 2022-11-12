class Win32Demo : public GameState
{
private:
	Transform3D* pCameraTransform;

public:
	Win32Demo() {}

	void start(BaseNode* pData, BaseNode* pSystems, BaseNode* pGUI)
	{
		cout << "Starting game state. \n";
	}

	void update(BaseNode* pData, float fDeltaTime) 
	{
		//	process windows messages
		//	Move to input handler

	}

	void render(OutputBuffer* pEngineBuffer)
	{
		//	Render2D renderer(pEngineBuffer, pCameraTransform->getPosition());
	}
};