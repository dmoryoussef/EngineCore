class EntityNode : public DataTreeNode<EntityNode>
{
private:
	BaseNode* pBaseNode;

public:
	EntityNode(BaseNode* pnode) :
		pBaseNode(pnode)
	{

	};

	void render()
	{

	}
};

class EntityComponentSystemDemo : public GameState
{
private:
	BaseNode* Root;

public:
	EntityComponentSystemDemo() :
		GameState() {};

	void start(BaseNode* pData, BaseNode* pSystems, BaseNode* pGUI) 
	{
		GameState::start(pData, pSystems, pGUI);
		
		Root = pSystems->getChild<EntityFactory>()->createPlayer(0);

		Joint* a = new Joint("a");
		a->addNode(new Transform2D({ 0.5, -0.71 }, { 0.5, 0.5 }, { .3, .3 }))->
		   addNode(new Render(Polygon2D(4)))->
		   addNode(new UIState());
		Root->addNode(a);

		Joint* b = new Joint("b");
		b->addNode(new Transform2D({ -0.5, -0.71 }, { .5, .5 }, { .3, .3 }))->
		   addNode(new Render(Polygon2D(4)))->
		   addNode(new UIState());
		Root->addNode(b);

		pData->addChild(Root);
	}

	void update(BaseNode* pData, float fDeltaTime) {}

	void render(OutputBuffer* pEngineBuffer) 
	{
		Render2D r(pEngineBuffer, cameraPos->getPosition());
	}
};