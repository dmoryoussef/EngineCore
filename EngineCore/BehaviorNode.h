enum BehaviorNodeState
{
	IDLE,
	RUNNING,
	FAILURE,
	SUCCESS
};

class BehaviorTreeBlackboard : public EventListener
{
	//	shared data for behavior tree nodes
	//	may be generalized later
	//	hashmap possibly with string::data
private:
	void onMouseWorldEvent(MouseWorldEvent* pEvent)
	{
		vTarget = pEvent->getWorldPosition();
	}

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case MOUSEWORLD_EVENT: onMouseWorldEvent(pEvent->get<MouseWorldEvent>());
				break;
		}
	}
public:
	BehaviorTreeBlackboard() 
	{
		registerListener(MOUSEWORLD_EVENT);
	}

	Vector2 vTarget;
	vector<Vector2> vPath;
	BaseNode* pTarget;
};

class BehaviorNode : public DataTreeNode<BehaviorNode>
{
protected:
	string name;
	int m_nState;

	BehaviorTreeBlackboard* Blackboard;

public:
	BehaviorNode(string n = "name", BehaviorTreeBlackboard* blackboard = NULL) :
		name(n),
		Blackboard(blackboard),
		m_nState(IDLE) {};

	void addChild(BehaviorNode* node)
	{
		node->Blackboard = Blackboard;
		m_vChildren.push_back(node);
	}

	string getName()
	{
		return name;
	}

	int getState()
	{
		return m_nState;
	}
	void setState(int state)
	{
		m_nState = state;
	}
	string stateToString()
	{
		switch (m_nState)
		{
			case IDLE: 
				return "IDLE";
				break;
			case RUNNING:
				return "RUNNING";
				break;
			case FAILURE:
				return "FAILURE";
				break;
			case SUCCESS:
				return "SUCCESS";
				break;
		}
	}
	string toString()
	{
		return name + ": " + stateToString() + description();
	}

	/*virtual void render(Render2D* renderer, int x, int y)
	{
		DataTreeNode::render(renderer, x, y);
	}*/
	virtual int update(float fDeltaTime) { return m_nState; }
	virtual int execute(float fDeltaTime) { return m_nState; }
	virtual string description() { return "Error: description function not implemented."; }

	virtual void renderNodeData(Render2D* renderer) {}
	virtual void renderNode(Render2D* renderer, Vector2 vMin, Vector2 vMax, Vector2 vSize)
	{
		//	special implementation of renderNode that changes color based on node state

		int color = FG_WHITE;
		switch (m_nState)
		{
			case RUNNING: color = FG_YELLOW;
				break;
			case SUCCESS: color = FG_LIGHTGREEN;
				break;
			case FAILURE: color = FG_LIGHTRED;
				break;
		}
		renderer->DrawQuad(vMin.X, vMin.Y, vMax.X, vMax.Y, { PIXEL_SOLID, color });
		if (renderer->vCameraTransform.Z > 7)
		{
			vector<string> nodeData;
			nodeData.push_back(name);
			nodeData.push_back(stateToString());
			nodeData.push_back(description());
			renderer->DrawString(nodeData, (vMin + (vSize * 0.1)).X, (vMin + (vSize / 5)).Y);
			//	renderer->DrawWrappedString(nodeData, (vMin + (vSize * 0.1)).X, (vMin + (vSize / 5)).Y, vSize.X * 0.8);
		}
	    else
		{
			renderer->DrawString(name, (vMin + (vSize * 0.1)).X, (vMin + (vSize / 2)).Y - (1.0 / renderer->vCameraTransform.Z));
		}

		//	also add this overridable function to render more specific node info
		renderNodeData(renderer);
	}
	
	virtual void reset(){}

	void resetChildren(int state = IDLE)
	{
		reset();
		for (auto c : m_vChildren)
		{
			c->setState(state);
			c->resetChildren();
		}
	}
};

//BehaviorTreeBlackboard BehaviorNode::Blackboard;

class DecoratorNode : public BehaviorNode
{
protected:
	BehaviorNode* child;


	int update(float fDeltaTime)
	{
		return child->update(fDeltaTime);
	}
	//	inverter
	//	succeeder
	//	repeater
	//	repeat until fail
	//

public:
	DecoratorNode(string n = "Decorator", BehaviorTreeBlackboard* blackboard = NULL) :
		BehaviorNode(n, blackboard) {};

};

class CompositeNode : public BehaviorNode
{
	//	Node types that have more than one child will derive from this
protected:
	BehaviorNode* current;
	int m_nCurrentIt;
public:
	CompositeNode(string n, BehaviorTreeBlackboard* blackboard = NULL) :
		m_nCurrentIt(0),
		current(NULL),
		BehaviorNode(n, blackboard) {};
};

class SequenceNode : public CompositeNode
{
	//	Attempts all children in order
	//	if any child fails this node will automatically fail

public:
	SequenceNode(string n = "SequenceNode", BehaviorTreeBlackboard* blackboard = NULL) :
		CompositeNode(n, blackboard) {};
	
	int update(float fDeltaTime)
	{
		if (m_nState == RUNNING)
		{
			if (current == NULL)
			{
				if (m_nCurrentIt == 0)
					current = getChildren()[m_nCurrentIt];
			}
			else
			{
				switch (current->getState())
				{
					case IDLE:
						current->setState(RUNNING);
						break;
					case RUNNING: 
						current->update(fDeltaTime);
						break;
					case FAILURE:
						//	if child fails, this node fails also.
						m_nState = FAILURE;
						resetChildren();
						break;
					case SUCCESS:
						//	if child succeeds check next child
						//	if next child -> set to current
						if (m_nCurrentIt < m_vChildren.size() - 1)
						{
							m_nCurrentIt++;
							current = getChildren()[m_nCurrentIt];
						}
						else
						{
							//	else: all children have succeeded
							m_nState = SUCCESS;
							resetChildren();
						}
						break;
				}
					
			}
		}

		return m_nState;
	}
	
	void reset()
	{
		current = NULL;
		m_nCurrentIt = 0;
	}

	string description()
	{
		if (m_nState == IDLE)
			return "Waiting to run.";

		if (m_nState == SUCCESS)
			return "Finished handling all children.";

		if (m_nState == FAILURE)
			return "Child " + thingToString<int>(m_nCurrentIt + 1) + " failed.";

		return "Handling child " + thingToString<int>(m_nCurrentIt + 1) + " of " + thingToString<int>(m_vChildren.size());
	}
};

class SelectorNode : public CompositeNode
{
	//	AKA "Fallback Node"
	//	falls-back to next node if current one fails
	// 
	//	Alternative to Sequence node
	//	Attempts all children in order
	//	if any child node fails, tries the next child
	//	this node only fails if all children node fails

public:
	SelectorNode(string n = "SelectorNode", BehaviorTreeBlackboard* blackboard = NULL) :
		CompositeNode(n) {};

	int update(float fDeltaTime)
	{
		if (m_nState == RUNNING)
		{
			if (current == NULL)
			{
				//	how to handle no children??
				//	auto fail or auto success??
				if (m_nCurrentIt == 0)
					current = getChildren()[m_nCurrentIt];
			}
			else
			{
				switch (current->getState())
				{
					case IDLE:
						current->setState(RUNNING);
						break;
					case RUNNING:
						current->update(fDeltaTime);
						break;
					case FAILURE:
						//	current failed:
						//	try next child
						if (m_nCurrentIt < m_vChildren.size() - 1)
						{
							m_nCurrentIt++;
							current = getChildren()[m_nCurrentIt];
						}
						else
							//	all children failed
							m_nState = FAILURE;
						break;
					case SUCCESS:
						//	any one child succeeded
						m_nState = SUCCESS;
						break;
				}

			}
		}

		return m_nState;
	}
};

class ParallelNode : public BehaviorNode
{
private:

public:
	ParallelNode(string n = "Parallel", BehaviorTreeBlackboard* blackboard = NULL) :
		BehaviorNode(n, blackboard) {};

	int update(float fDeltaTime)
	{
		switch (m_nState)
		{
			case RUNNING:
				for (auto c : m_vChildren)
				{
					int current = c->update(fDeltaTime);
					switch (current)
					{
						case FAILURE:
							m_nState == FAILURE;
							c->reset();
							break;
						case SUCCESS:
							c->reset();
							break;

					} 
						
				}
				break;
		}

		return m_nState;
	}
};

class LeafNode : public BehaviorNode
{
public:
	LeafNode(string n = "Leaf", BehaviorTreeBlackboard* blackboard = NULL) :
		BehaviorNode(n, blackboard) {};

	int update(float fDeltaTime)
	{
		switch (m_nState)
		{
			case IDLE:
				m_nState = RUNNING;
				break;
			case RUNNING:
				//	do specific thing
				m_nState = execute(fDeltaTime);
				break;
			case FAILURE:
				//	notify parent of failure
				break;
			case SUCCESS:
				//	notify parent of success
				break;
		}

		return m_nState;
	}

};