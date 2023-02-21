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

		Joint* j = new Joint();
		Root->addNode(j);
		j->addNode(new Transform2D({ 0, -1 }, { 0, 0 }, { 1, 1 }))->
		   addNode(new Render(Polygon2D(4)));


		pData->addChild(Root);
	}

	void update(BaseNode* pData, float fDeltaTime) {}

	void render(OutputBuffer* pEngineBuffer) 
	{
		Render2D r(pEngineBuffer, cameraPos->getPosition());
	}
};