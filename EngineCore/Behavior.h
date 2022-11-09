class Behavior : public _EntityComponent
{
private:
	BehaviorNode* BehaviorTree;

public:
	Behavior(BehaviorNode *tree) :
		BehaviorTree(tree),
		_EntityComponent("BEHAVIOR") {};

	void update(float fDeltaTime)
	{
		BehaviorTree->update(fDeltaTime);
	}


};