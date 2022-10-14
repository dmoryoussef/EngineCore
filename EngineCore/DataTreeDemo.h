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
		int height = pGUI->cast<_UIComponent>()->getHeight();
		int width = pGUI->cast<_UIComponent>()->getWidth();
		CameraViewWindow* pCameraWindow = new CameraViewWindow(width, height, 0, 0);
		BaseNode* pCamera = pCameraWindow->getCamera();
		pCameraTransform = pCamera->getChild<Transform3D>();
		pData->add(pCamera);
		pGUI->addAtEnd(pCameraWindow);

		BehaviorNode* nodeD = new BehaviorNode();
		nodeD->addChild(new BehaviorNode());
		nodeD->addChild(new BehaviorNode());
		nodeD->addChild(new BehaviorNode());

		BehaviorNode* nodeE = new BehaviorNode();
		nodeE->addChild(new BehaviorNode());
		nodeE->addChild(new BehaviorNode());
		
		BehaviorNode* nodeA = new BehaviorNode();
		nodeA->addChild(new BehaviorNode());
		nodeA->addChild(new BehaviorNode());
		nodeA->addChild(new BehaviorNode());

		BehaviorNode* nodeB = new BehaviorNode();
		nodeB->addChild(new BehaviorNode());
		nodeB->addChild(nodeD);
		

		BehaviorNode* nodeC = new BehaviorNode();
		nodeC->addChild(new BehaviorNode());
		nodeC->addChild(new BehaviorNode());
		nodeC->addChild(new BehaviorNode());
		nodeC->addChild(nodeE);


		tree = new BehaviorNode();
		tree->addChild(nodeA);
		tree->addChild(nodeB);
		tree->addChild(nodeC);
	}

	void update(BaseNode* pData, float fDeltaTime)
	{

	}

	void render(OutputBuffer* pEngineBuffer)
	{
		Render2D renderer(pEngineBuffer, pCameraTransform->getPosition());
		tree->render(&renderer, 5, 5);

		//	renderer.DrawString("Total Leaf Nodes: " + thingToString<int>(tree->getTotalLeafNodes(0, tree)), 4, 5);
	}
};