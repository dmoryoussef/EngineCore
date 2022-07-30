

class BuildingTile : public _Tile2D<BuildingTile>
{
private:
public:
	BuildingTile() :
		_Tile2D("BUILDING_TILE") {}
};

class BuildingMap : public _TileMap<BuildingTile>
{
private:
	//	building map specific algorithms
	//		flood fill
	//		tile events?
	//	mouse interation
	//		building, selecting tiles
	//		group selection

	int nCurrentBuildMode;

	void onEvent(_Event *pEvent)
	{
		_TileMap::onEvent(pEvent);

		switch (pEvent->m_eType)
		{
			case GUI_EVENT:
			{
				if (pEvent->m_eType == GUI_EVENT)
				{
					if (pEvent->get<GuiEvent>()->getComponent<UIButton>()->getState() == LEFT_RELEASED)
					{
						if (pEvent->get<GuiEvent>()->getComponent<UIButton>()->getText() == "POINT")
						{
							nCurrentBuildMode = 0;
						}

						if (pEvent->get<GuiEvent>()->getComponent<UIButton>()->getText() == "QUAD")
						{
							nCurrentBuildMode = 1;
						}
					}
				}
				break;
			}

			case MOUSEWORLD_EVENT:
			{
				MouseWorldEvent* pMouseEvent = pEvent->get<MouseWorldEvent>();
				if (BuildingTile* pTile = getWorldTile(pMouseEvent->getWorldPosition().X, pMouseEvent->getWorldPosition().Y))
				{
					if (pMouseEvent->getState().bRightButtonDown)
					{
						pTile->setValue(0.1);
					}
				}
				break;
			}

			case SELECTIONSQUARE_EVENT:
			{
				if (!pEvent->get<SelectionSquareEvent>()->isActive())
				{
					SelectionSquareEvent* pSelectionEvent = pEvent->get<SelectionSquareEvent>();
					Vector2 vMin = pSelectionEvent->getMin();
					Vector2 vMax = pSelectionEvent->getMax();

					//	fill square

					//	draw lerped line
					if (nCurrentBuildMode == 0)
					{
						Vector2 vStart = pSelectionEvent->getStart();
						Vector2 vStop = pSelectionEvent->getStop();
						for (float t = 0; t < 1; t = t + 0.01)
						{
							Vector2 vLerp = lerp(vStart, vStop, t);
							if (BuildingTile* pTile = getWorldTile(vLerp.X, vLerp.Y))
								pTile->setValue(1);
						}
					}

					//	draw square
					if (nCurrentBuildMode == 1)
					{
						for (int y = vMin.Y; y < vMax.Y; y++)
						{
							for (int x = vMin.X; x < vMax.X; x++)
							{
								if (x == vMin.X || x == vMax.X - 1 || y == vMin.Y || y == vMax.Y - 1)
									if (BuildingTile* pTile = getWorldTile(x, y))
										pTile->setValue(1);
							}
						}
					}

				}
				break;
			}
		}
	}

public:
	BuildingMap() :
		nCurrentBuildMode(1),
		_TileMap({ 100, 100 }, "BUILDING_MAP") 
	{
		setPosition(5, 5);
		createBoarderMap();
		registerListener(GUI_EVENT);
		registerListener(SELECTIONSQUARE_EVENT);
	}
};

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
	BezierPath bezier;
	Transform3D* pCamera;

public:
	BuildingTest() :
		bezier()
	{
		registerListener(MOUSEWORLD_EVENT);
		registerListener(KEYBOARD_EVENT);
		registerListener(GUI_EVENT);
	};


	void start(BaseNode* pData, BaseNode* pSystems, BaseNode* pGUI)
	{
		CameraViewWindow* pCameraWindow = new CameraViewWindow(180, 150, 0, 0);
		pData->add(pCameraWindow->getCamera());
		pCamera = pCameraWindow->getCamera()->getChild<Transform3D>();
		pGUI->addChild(pCameraWindow);

		//SingleSelectButtonComponent* pComponent = new SingleSelectButtonComponent();
		//pComponent->addComponent(new UIButton("POINT"));
		//pComponent->addComponent(new UIButton("QUAD"));
		//pComponent->addComponent(new UIButton("CURVE"));
		//pComponent->refresh();
		//pCameraWindow->addComponent(pComponent);
		//pComponent->setAlignment(ALIGN_RIGHT);

		//pData->add(new BuildingMap());
	}

	void render(OutputBuffer* pEngineBuffer)
	{
		//	currently renders outside "window" becuase there is no bounding information
		//	move to window to fix??
		Render2D renderer(pEngineBuffer);
		bezier.render(&renderer, pCamera->getPosition());
	}

};