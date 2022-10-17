enum BehaviorNodeState
{
	IDLE,
	RUNNING,
	FAILURE,
	SUCCESS
};

class BehaviorNode : public DataTreeNode<BehaviorNode>
{
protected:
	string name;
	int m_nState;

public:
	BehaviorNode(string n = "name") :
		name(n),
		m_nState(IDLE) {};

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
	virtual int update(float fDeltaTime) { return 0; }
	virtual int execute(float fDeltaTime) { return 0; }
	virtual string description() { return "Error: function not implemented."; }

	void renderNode(Render2D* renderer, Vector2 vMin, Vector2 vMax, Vector2 vSize)
	{
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
		//if (renderer->vCameraTransform.Z > 15)
		{
			vector<string> nodeData;
			nodeData.push_back(name);
			nodeData.push_back(stateToString());
			nodeData.push_back(description());
			renderer->DrawString(nodeData, (vMin + (vSize * 0.1)).X, (vMin + (vSize / 5)).Y);
			// renderer->DrawWrappedString(description(), (vMin + (vSize * 0.1)).X, (vMin + (vSize / 5)).Y, vSize.X * 0.8);
		}
	/*	else
		{
			renderer->DrawString(name, (vMin + (vSize * 0.1)).X, (vMin + (vSize / 5)).Y);
		}*/
	}
	
	virtual void reset(){}

	void resetChildren()
	{
		m_nState = RUNNING;
		reset();
		for (auto c : m_vChildren)
		{
			c->resetChildren();
		}
	}
};

class DecoratorNode : public BehaviorNode
{
protected:
	BehaviorNode* child;

	int update(float fDeltaTime)
	{
		child->update(fDeltaTime);
	}
	//	inverter
	//	succeeder
	//	repeater
	//	repeat until fail
	//

public:
	DecoratorNode(string n = "Decorator") :
		BehaviorNode(n) {};

};

class CompositeNode : public BehaviorNode
{
	//	Node types that have more than one child will derive from this
protected:
	BehaviorNode* current;
	int m_nCurrentIt;
public:
	CompositeNode(string n) :
		m_nCurrentIt(0),
		current(NULL),
		BehaviorNode(n) {};
};

class SequenceNode : public CompositeNode
{
	//	Attempts all children in order
	//	if any child fails this node will automatically fail

public:
public:
	SequenceNode(string n = "SequenceNode") :
		CompositeNode(n) {};
	
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
							//	else: all children have succeeded
							m_nState = SUCCESS;
						break;
				}
					
			}
		}

		return m_nState;
	}
		
	string description()
	{
		if (m_nState == SUCCESS)
			return "Finished handling all children.";

		if (m_nState == FAILURE)
			return "Child " + thingToString<int>(m_nCurrentIt + 1) + " failed.";

		return "Handling child " + thingToString<int>(m_nCurrentIt + 1) + " of " + thingToString<int>(m_vChildren.size());
	}
};

class SelectorNode : public CompositeNode
{
	//	Alternative to Sequence node
	//	Attempts all children in order
	//	if any child node fails, tries the next child
	//	this node only fails if all children node fails

public:
	SelectorNode(string n = "SelectorNode") :
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

class LeafNode : public BehaviorNode
{
public:
	LeafNode(string n = "Leaf") :
		BehaviorNode(n) {};

	int update(float fDeltaTime)
	{
		switch (m_nState)
		{
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