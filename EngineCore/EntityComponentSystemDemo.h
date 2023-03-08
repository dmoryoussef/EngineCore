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
	BaseNode* pTest;
	BaseNode* pGameData;

	BaseNode *createEntity(int id)
	{
		BaseNode *Root = new BaseNode("PLAYER " + thingToString<int>(id + 1));
		Root->addNode(new Render(Polygon2D(3)))->
			  addNode(new Transform2D({ float(random(1, 10)), float(random(1, 10)) }, { 0, 1 }, { 1, 1 }))->
			  addNode(new DetailsPanel())->
			  addNode(new Velocity())->
			  addNode(new UserController(id))->
			  addNode(new ShootAction(100000, 200.0))->
			  addNode(new Collider2D(Polygon2D(3, 1.2)))->
			  addNode(new UIState())->
			  addNode(new Health(100));

		Joint* a = new Joint("ENGINE");
		a->addNode(new Transform2D({ 0.5, -0.71 }, { 0.5, 0.5 }, { .3, .3 }))->
		   addNode(new Render(Polygon2D(4)))->
		   addNode(new UIState())->
		   addNode(new Accelerate(0.0001));
		Root->addNode(a);

		Joint* b = new Joint("ENGINE");
		b->addNode(new Transform2D({ -0.5, -0.71 }, { .5, .5 }, { .3, .3 }))->
		   addNode(new Render(Polygon2D(4)))->
		   addNode(new UIState())->
		   addNode(new Accelerate(0.0001));
		Root->addNode(b);

		/*Joint* c = new Joint("SHIELDS");
		c->addNode(new Transform2D({0, 0}, {0, 0}, {1, 1}))->
			addNode(new Render(Polygon2D(12, 2)))->
			addNode(new UIState());
		Root->addNode(c);*/

		addEvent(new NewBaseNodeEvent(Root));
		return Root;
	}

public:
	EntityComponentSystemDemo() :
		GameState() {};

	void start(BaseNode* pData, BaseNode* pSystems, BaseNode* pGUI) 
	{
		pGameData = pData;
		GameState::start(pData, pSystems, pGUI);
		pTest = createEntity(0);
		pData->add(pTest);
		pData->add(createEntity(1));

	}

	void update(BaseNode* pData, float fDeltaTime) {}

	void render(OutputBuffer* pEngineBuffer) 
	{
		Render2D r(pEngineBuffer);
		r.DrawNum<int>(pGameData->getTotal(), 5, 4);
		if (Velocity *pVelocity = pTest->getChild<Velocity>())
			r.DrawString(thingToString<float>(pVelocity->getVelocity().magnitude()), 5, 5);

	}
};