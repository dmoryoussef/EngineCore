class DataTreeDemo : public GameState
{
private:
	Transform3D* pCameraTransform;
	BehaviorNode* tree;

public:
	DataTreeDemo() {};
	~DataTreeDemo()
	{
		delete tree;
	}

	void start(BaseNode* pData, BaseNode* pSystems, BaseNode* pGUI)
	{
		
	
	}

	void update(BaseNode* pData, float fDeltaTime)
	{
		tree->update(fDeltaTime);
	}

	void render(OutputBuffer* pEngineBuffer)
	{
		Render2D renderer(pEngineBuffer, pCameraTransform->getPosition());
		tree->render(&renderer, 5, 5);
	}
};