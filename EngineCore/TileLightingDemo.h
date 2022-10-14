//	will need system to apply light value after checking collision map for line of sight
//	for each tile:
//		for each light:
//			applyLight(lightPosition, lightValue, lightMap, collisionMap)
// 
//	look for ways to make it a seperate thread 
//	look for ways to only update after a light entity has moved

class TileLightingDemo : public GameState
{
private:
	Vector2 LightPosition;
	LightMap *m_pLightMap;

	void onMouseWorldEvent(MouseWorldEvent* pEvent)
	{
		LightPosition = pEvent->getWorldPosition() - Vector2(0.5, 0.5);
	}

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case MOUSEWORLD_EVENT: onMouseWorldEvent(pEvent->get<MouseWorldEvent>());
				break;
		}
	}

public:
	TileLightingDemo() :
		m_pLightMap(NULL),
		LightPosition(),
		GameState() 
	{
		registerListener(MOUSEWORLD_EVENT);
	};

	void start(BaseNode* pData, BaseNode* pSystems, BaseNode* pGUI)
	{
		int height = pGUI->cast<_UIComponent>()->getHeight();
		int width = pGUI->cast<_UIComponent>()->getWidth();
		CameraViewWindow* pCameraWindow = new CameraViewWindow(width, height, 0, 0);
		BaseNode* pCamera = pCameraWindow->getCamera();
		pData->add(pCamera);
		pGUI->addAtEnd(pCameraWindow);

		m_pLightMap = new LightMap(50, 50);
		pData->add(m_pLightMap);
	}

	void render(OutputBuffer* pEngineBuffer) 
	{
		Render2D renderer(pEngineBuffer);
	}

	void update(BaseNode* pData, float fDeltaTime) 
	{
		m_pLightMap->applyLight(LightPosition, 2.0);
	}
};