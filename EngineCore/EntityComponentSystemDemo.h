class EntityComponentSystemDemo : public GameState
{
private:
	BaseNode* pTest;
	BaseNode* pGameData;
	BehaviorNode* Tree;

	void addAI(BaseNode* pEntity, BaseNode* pData)
	{
		RepeatDecorator* TreeBase = new RepeatDecorator("Root Node", new BehaviorTreeBlackboard(pEntity, pData));
		TreeBase->setState(RUNNING);

		BehaviorNode* sequenceA = new SequenceNode();
		TreeBase->addNode(sequenceA);
		sequenceA->addNode(new GetTarget())->
				   addNode(new FleeBehavior())->
				   addNode(new SeekBehavior())->
				   addNode(new AttackBehavior());


		pEntity->addNode(new Behavior(TreeBase));
		pEntity->remove<UserController>();
		Tree = TreeBase;
	}

	BaseNode *createEntity(int id)
	{
		BaseNode *Root = new BaseNode("PLAYER " + thingToString<int>(id + 1));
		Root->addNode(new Render(Polygon2D(3)))->
			  addNode(new Transform2D({ float(random(1, 15)), float(random(1, 15)) }, { 1, 0 }, { 1, 1 }))->
			  addNode(new DetailsPanel())->
			  addNode(new Velocity())->
			  addNode(new UserController(id, DIRECT))->
			  addNode(new ShootAction(100000, 200.0))->
			  addNode(new Collider2D(Polygon2D(3, 1.2)))->
			  addNode(new UIState())->
			  addNode(new Health(100));

		Joint* a = new Joint("ENGINE");
		a->addNode(new Transform2D({ 0.5, 0.71 }, { 0.5, 0.5 }, { .3, .3 }))->
		   addNode(new Render(Polygon2D(4)))->
		   addNode(new UIState())->
		   addNode(new Accelerate(0.0001));
		Root->addNode(a);

		Joint* b = new Joint("ENGINE");
		b->addNode(new Transform2D({ -0.5, 0.71 }, { .5, .5 }, { .3, .3 }))->
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
		GameState::start(pData, pSystems, pGUI);
		pGameData = pData;
		pTest = createEntity(0);
		pData->add(pTest);
		BaseNode* pEnemy = createEntity(1);
		pData->add(pEnemy);
		addAI(pEnemy, pData);
	}

	void update(BaseNode* pData, float fDeltaTime) {}

	void render(OutputBuffer* pEngineBuffer) 
	{
		Render2D r(pEngineBuffer);
		Tree->render(&r, 25, 2, 8);
	}
};