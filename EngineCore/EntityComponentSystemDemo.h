class EntityComponentSystemDemo : public GameState
{
private:
	BaseNode* Player;
	BaseNode* AI;
	BaseNode* pGameData;
	BehaviorNode* Tree;

	BaseNode* stars;
	int totalStars;

	Vector2 screenCenter;

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
			  addNode(new Velocity(0.5))->
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

	void createStarfield(int n)
	{
		stars = new BaseNode[n];
		totalStars = n;
		for (int i = 0; i < totalStars; i++)
		{
			int rP = random(0, 3);
			int p = 0;
			switch (rP)
			{
				case 0: p = { PIXEL_QUARTER }; break;
				case 1: p = { PIXEL_HALF }; break;
				case 2: p = { PIXEL_THREEQUARTER }; break;
				case 3: p = { PIXEL_SOLID }; break;
			}
			int rC = random(0, 2);
			int c = 0;
			switch (rC)
			{
				case 0: c = { FG_YELLOW }; break;
				case 1: c = { FG_WHITE }; break;
				case 2: c = { FG_LIGHTGREY }; break;
			}
			Pixel color = { p , c };
			stars[i].addNode(new Transform2D({ float(random(1, 500) - 250), float(random(1, 500) -250) }, { 0, 0 }, { 1, 1 }));
			stars[i].addNode(new Render(Polygon2D(1), color));
		}
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

		createStarfield(250);
	}

	void update(BaseNode* pData, float fDeltaTime)
	{
		Vector2 positionA = Player->getChild<Transform2D>()->getPosition();
		Vector2 positionB = AI->getChild<Transform2D>()->getPosition();
		Vector3 camPos = camera->getChild<Transform3D>()->getPosition();
		
		screenCenter = Vector2(ScreenWidth/2, ScreenHeight/2);
		screenCenter.div(camPos.Z);

		float z = camPos.Z;
		float x = positionA.X * z;
		float y = positionA.Y * z;
		camPos = { -x + screenCenter.X, -y + screenCenter.Y, z };
		camera->getChild<Transform3D>()->setPosition(camPos);
	}

	void render(OutputBuffer* pEngineBuffer) 
	{
		Render2D r(pEngineBuffer, camera->getChild<Transform3D>()->getPosition());
		//	Tree->render(&r, 25, 2, 8);
		for (int i = 0; i < totalStars; i++)
		{
			mat3x3 mat = stars[i].getChild<Transform2D>()->getTransformMatrix();
			Render* render = stars[i].getChild<Render>();
			vector<Vector2> trans = render->getPolygon().transformedVerts(mat);
			Pixel c = render->getColor();
			r.DrawPoly(trans, c);
		}



		Render2D r2(pEngineBuffer);

		r2.DrawString(screenCenter.toString(), 50, 5);

		vector<string> PlayerData;
		PlayerData.push_back(Player->getName());
		if (Health* pHealth = Player->getChild<Health>())
			PlayerData.push_back(thingToString(pHealth->getHealth()));

		r2.DrawString(PlayerData, 5, 5);

		vector<string> AiData;
		AiData.push_back(AI->getName());
		if (Health* pHealth = AI->getChild<Health>())
			AiData.push_back(thingToString(pHealth->getHealth()));

		r2.DrawString(AiData, 5, r2.getSize().Y - 8);
		


	}
};