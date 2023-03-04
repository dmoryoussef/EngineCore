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
		
		int id = 0;
		string s = "PLAYER ";
		s = s.append(thingToString<int>(id));
		Polygon2D shape;
		shape.createPoly(3);

		Root = new BaseNode(s);
		Root->addNode(new Render(shape))->
			addNode(new Transform2D({ float(random(1, 10)), float(random(1, 10)) }, { 1, 0 }, { 1, 1 }))->
			addNode(new Velocity())->
			addNode(new UserController(id))->
			addNode(new ShootAction(100000, 200.0))->
			addNode(new Collider2D(shape))->
			addNode(new UIState())->
			addNode(new Health(100));
			  
		Joint* a = new Joint("a");
		a->addNode(new Transform2D({ 0.5, -0.71 }, { 0.5, 0.5 }, { .3, .3 }))->
		   addNode(new Render(Polygon2D(4)))->
		   addNode(new UIState())->
			addNode(new Accelerate(0.0));
		Root->addNode(a);

		Joint* b = new Joint("b");
		b->addNode(new Transform2D({ -0.5, -0.71 }, { .5, .5 }, { .3, .3 }))->
		   addNode(new Render(Polygon2D(4)))->
		   addNode(new UIState())->
			addNode(new Accelerate(0.0));
		Root->addNode(b);

		pData->add(Root);
		addEvent(new NewBaseNodeEvent(Root));
	}

	void update(BaseNode* pData, float fDeltaTime) {}

	void render(OutputBuffer* pEngineBuffer) 
	{
		Render2D r(pEngineBuffer, cameraPos->getPosition());
	}
};