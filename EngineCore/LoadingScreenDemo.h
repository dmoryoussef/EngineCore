class LoadingScreenDemo : public GameState
{
private:
	vector<GameState*> States;

public:
	LoadingScreenDemo() {}

	void init(vector<GameState*> states)
	{
		States = states;
	}

	void start(BaseNode* pData, BaseNode* pSystems, BaseNode* pGUI)
	{
		int height = pGUI->cast<_UIComponent>()->getHeight();
		int width = pGUI->cast<_UIComponent>()->getWidth();
		UIWindow* window = new UIWindow(width, height, 0, 0);
		pGUI->add(window);


		for (GameState* s : States)
		{
			UIButton* button = new UIButton(s->getName());
			button->setAlignment(ALIGN_CENTER);
			window->addComponent(button);
		}
	}

	void update(BaseNode* pData, float fDeltaTime)
	{

	}

	void render(OutputBuffer* pEngineBuffer)
	{

	}
};