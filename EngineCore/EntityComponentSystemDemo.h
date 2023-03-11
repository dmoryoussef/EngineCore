class EntityComponentSystemDemo : public GameState
{
private:
	BaseNode* Player;
	BaseNode* AI;
	BaseNode* pGameData;
	BehaviorNode* Tree;

	BaseNode stars[500];

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
		Root->addNode(new Render(Polygon2D(3), {PIXEL_SOLID, FG_LIGHTGREY }))->
			  addNode(new Transform2D({ float(random(1, 15)), float(random(1, 15)) }, { 1, 0 }, { 1, 1 }))->
			  addNode(new DetailsPanel())->
			  addNode(new Velocity(0.1))->
			  addNode(new UserController(id, DIRECT))->
			  addNode(new ShootAction(10000, 500.0))->
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
		Player = createEntity(0);
		pData->add(Player);
		AI = createEntity(1);
		pData->add(AI);
		addAI(AI, pData);

		for (int i = 0; i < 500; i++)
		{
			stars[i].addNode(new Transform2D({ float(random(1, 500)), float(random(1, 500)) }, { 0, 0 }, { 1, 1 }));
			stars[i].addNode(new Render(Polygon2D(1)));
		}
	}

	void update(BaseNode* pData, float fDeltaTime)
	{
		//Vector2 positionA = Player->getChild<Transform2D>()->getPosition();
		//Vector2 positionB = AI->getChild<Transform2D>()->getPosition();
		//Vector3 camPos = camera->getChild<Transform3D>()->getPosition();
		//
		//float x = positionA.X;
		//float y = positionA.Y;
		//float z = camPos.Z;// *0.9;
		//camPos = { x, y, z };
		//camera->getChild<Transform3D>()->setPosition(camPos);
		
	}

	void render(OutputBuffer* pEngineBuffer) 
	{
		Render2D r(pEngineBuffer, camera->getChild<Transform3D>()->getPosition());
		//	Tree->render(&r, 25, 2, 8);
		for (int i = 0; i < 500; i++)
		{
			mat3x3 mat = stars[i].getChild<Transform2D>()->getTransformMatrix();
			mat3x3 matOffset = matOffset.Translate(Vector2(-250, -250));
			mat = matOffset * mat;
			vector<Vector2> trans = stars[i].getChild<Render>()->getPolygon().transformedVerts(mat);

			r.DrawPoly(trans, {PIXEL_HALF, FG_WHITE});
		}
	}
};