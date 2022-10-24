

class LivePolyEditor : public GameState
{
private:
	SelectionSquare* selectionSquare;
	Transform3D* pCameraTransform;
	bool bSelectionSquareHotkey;
	PolyList Polys;

	void onSelectionLineEvent(SelectionLineEvent* pEvent)
	{
		if (pEvent->isReleased())
		{
			delete selectionSquare;
			selectionSquare = NULL;
		}
	}

	void onSelectionSquareEvent(SelectionSquareEvent* pEvent)
	{
		if (pEvent->isReleased())
		{
			delete selectionSquare;
			selectionSquare = NULL;
		}
	}

	void onMouseWorldEvent(MouseWorldEvent* pEvent)
	{
		Vector2 pos = pEvent->getWorldPosition();
		if (pEvent->getState().bLeftButtonDown && bSelectionSquareHotkey)
		{
			if (!selectionSquare)
				selectionSquare = new SelectionSquare(pos, false, true);
		}

		if (pEvent->getState().bLeftButtonDown)
		{
			if (!selectionSquare)
				selectionSquare = new SelectionSquare(pos, true, false);
		}
	}
	
	void onKeyboardEvent(KeyboardEvent* pEvent)
	{
		switch (pEvent->getKey())
		{
			case 'p': bSelectionSquareHotkey = pEvent->isKeyDown('p');
				break;
		}
	}

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case SELECTIONLINE_EVENT: onSelectionLineEvent(pEvent->get<SelectionLineEvent>());
				break;
			case SELECTIONSQUARE_EVENT: onSelectionSquareEvent(pEvent->get<SelectionSquareEvent>());
				break;
			case MOUSEWORLD_EVENT: onMouseWorldEvent(pEvent->get<MouseWorldEvent>());
				break;
			case KEYBOARD_EVENT: onKeyboardEvent(pEvent->get<KeyboardEvent>());
				break;
		}
	}

public:
	LivePolyEditor() :
		bSelectionSquareHotkey(false),
		selectionSquare(NULL),
		Polys()
	{
		registerListener(KEYBOARD_EVENT);
		registerListener(SELECTIONSQUARE_EVENT);
		registerListener(SELECTIONLINE_EVENT);
		registerListener(MOUSEWORLD_EVENT);
	};


	void start(BaseNode * pData, BaseNode * pSystems, BaseNode * pGUI)
	{
		int height = pGUI->cast<_UIComponent>()->getHeight();
		int width = pGUI->cast<_UIComponent>()->getWidth();
		CameraViewWindow* pCameraWindow = new CameraViewWindow(width, height, 0, 0);
		BaseNode* pCamera = pCameraWindow->getCamera();
		pCameraTransform = pCamera->getChild<Transform3D>();
		pData->add(pCamera);
		pGUI->addAtEnd(pCameraWindow);



		pData->add(new BuildingMap(200, 100));
	}

	void render(OutputBuffer* pEngineBuffer)
	{
		Render2D renderer(pEngineBuffer, pCameraTransform->getPosition());

		if (selectionSquare) selectionSquare->render(&renderer);

		Polys.render(&renderer);
	}
};