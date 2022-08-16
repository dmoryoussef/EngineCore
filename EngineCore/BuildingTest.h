class BuildingTest : public GameState
{
	//	this test state uses the prebuilt engine and entity components
	//	and adds some custom code inside its own class 
	//	to implement user defined behavior beyond the scope of the existing engine

	//	room - 2d array
	//	floors - list of rooms
	//	buildings - list of floors
	//		forms a stack: foundation, basement, 1st floor, 2nd floor, etc, attic, roof, etc

	//	flood fill to assign rooms
	//		heat contained in room
	//			produced by "hearth" type area
	//	"weather" kept out of rooms
	//		unless door/window is open?
	// 
	//	floor tile
	//	wall tile
	// 
	//	tiles that are interactive
	//		door - open/close
	//			allow light to pass
	//			allow pathfinding
	//		window - open/close
	//			allow light
	//			block pathfinding
	//		stairs
	//			increase/decrease z-value of entity
	
	//	render modes
	//		floor/level to render
	//		render inside only "active" building
	//		render only top of other buildings
	//		build mode - highlight important info


private:

public:
	BuildingTest()
	{
		registerListener(MOUSEWORLD_EVENT);
		registerListener(KEYBOARD_EVENT);
		registerListener(GUI_EVENT);
	};


	void start(BaseNode* pData, BaseNode* pSystems, BaseNode* pGUI)
	{
		int height = pGUI->cast<_UIComponent>()->getHeight();
		int width = pGUI->cast<_UIComponent>()->getWidth();
		CameraViewWindow* pCameraWindow = new CameraViewWindow(width, height, 0, 0);
		pData->add(pCameraWindow->getCamera());
		pGUI->addChild(pCameraWindow);

		//SingleSelectButtonComponent* pComponent = new SingleSelectButtonComponent();
		//pComponent->addComponent(new UIButton("POINT"));
		//pComponent->addComponent(new UIButton("QUAD"));
		//pComponent->addComponent(new UIButton("CURVE"));
		//pComponent->refresh();
		//pCameraWindow->addComponent(pComponent);
		//pComponent->setAlignment(ALIGN_RIGHT);

		pData->add(new BuildingMap());
	}

	void render(OutputBuffer* pEngineBuffer)
	{
		Render2D renderer(pEngineBuffer);
	}

};