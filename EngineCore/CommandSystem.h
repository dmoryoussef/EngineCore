class EntityCommandSystem : public BaseNode
{
private:
	void onEvent(_Event* pEvent)
	{
		//	case command event
		//		shootAction:
		//			get action->execute()
		//	case accelerate command
		//		get transform, physics
		//			add accelerate force
	}

public:
	EntityCommandSystem() :
		BaseNode("Command System") 
	{
		//	register command event
	};


};