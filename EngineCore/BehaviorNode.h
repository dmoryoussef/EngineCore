enum BehaviorNodeState
{
	RUNNING,
	FAILURE,
	SUCCESS
};

class BehaviorNode : public DataTreeNode<BehaviorNode>
{
protected:
	string name;
	int m_nState;
	//BehaviorNode* parent;
	//vector<BehaviorNode*> children;

public:
	BehaviorNode(string n = "name") :
		name(n),
		m_nState(RUNNING) {};

	string getName()
	{
		return name;
	}

	int getState()
	{
		return m_nState;
	}

	string stateToString()
	{
		switch (m_nState)
		{
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
	virtual void run() {}
	virtual void execute() {}
	virtual string description() { return "Error: function not implemented."; }

	void renderNode(Render2D* renderer, Vector2 vMin, Vector2 vMax, Vector2 vSize)
	{
		renderer->DrawQuad(vMin.X, vMin.Y, vMax.X, vMax.Y, { PIXEL_SOLID, FG_WHITE });
		if (renderer->vCameraTransform.Z > 15)
		{
			vector<string> nodeData;
			nodeData.push_back(name);
			nodeData.push_back(stateToString());
			nodeData.push_back(description());
			renderer->DrawString(nodeData, (vMin + (vSize / 8)).X, (vMin + (vSize / 5)).Y);
		}
		else
		{
			renderer->DrawString(name, (vMin + (vSize / 8)).X, (vMin + (vSize / 5)).Y);
		}
	}

	//void render(Render2D *renderer, int x, int y)
	//{
	//	x = x + 2;
	//	renderer->DrawString(toString(), x, y);
	//	for (BehaviorNode* n : children)
	//	{
	//		y++;
	//		n->render(renderer, x, y);
	//	}
	//}
};

class DecoratorNode : public BehaviorNode
{
protected:
	BehaviorNode* child;

	void run()
	{
		child->run();
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
protected:
	BehaviorNode* current;

public:
	CompositeNode(string n) :
		current(NULL),
		BehaviorNode(n) {};
};


class SequenceNode : public CompositeNode
{
public:
	int m_nCurrentIt;
public:
	SequenceNode(string n = "SequenceNode") :
		m_nCurrentIt(0),
		CompositeNode(n) {};
	
	void run()
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
					case RUNNING: 
						current->run();
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
	}
		
	string description()
	{
		return "Handling child " + thingToString<int>(m_nCurrentIt + 1) + " of " + thingToString<int>(m_vChildren.size()) + ": ";
	}
};

class SelectorNode : public CompositeNode
{
public:
	SelectorNode(string n = "SelectorNode") :
		CompositeNode(n) {};

	void run()
	{
		for (BehaviorNode* n : getChildren())
		{
			n->run();
		}
		//	for each node
		//		try nodes in order
		//			if SUCCESS -> return SUCCESS
		//			if FAILURE -> try next node
		//				if no more nodes -> return FAILURE
		//
	}
};

class LeafNode : public BehaviorNode
{
public:
	LeafNode(string n = "Leaf") :
		BehaviorNode(n) {};

	void run()
	{
		switch (m_nState)
		{
			case RUNNING:
				//	do specific thing
				execute();
				break;
			case FAILURE:
				//	notify parent of failure
				break;
			case SUCCESS:
				//	notify parent of success
				break;
		}
	}

	void execute()
	{
		//	to be overridden 
	}


};

class BehaviorTree
{
private:

public:
	BehaviorTree() {};

	BehaviorNode* root;
	void run()
	{
		root->run();
	}

	void update()
	{
		run();
	}
};