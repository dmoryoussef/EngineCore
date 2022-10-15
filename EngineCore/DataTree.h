template<typename T> class DataTreeNode
{
protected:
	DataTreeNode* m_pParent;
	vector<T*> m_vChildren;

public:
	DataTreeNode() :
		m_vChildren(),
		m_pParent(NULL) {}
	~DataTreeNode()
	{
		deleteChildren();
	}

	int getTotalLeafNodes(int total, DataTreeNode *current)
	{
		if (current->getChildren().size() > 0)
		{
			for (DataTreeNode* c : current->getChildren())
				total = getTotalLeafNodes(total, c);
		}
		else
			total++;

		return total;
	}

	vector<T*> getChildren()
	{
		return m_vChildren;
	}

	void setParent(DataTreeNode* p)
	{
		m_pParent = p;
	}
		
	void deleteChildren()
	{
		for (DataTreeNode* n : m_vChildren)
		{
			n->deleteChildren();

			delete n;
			n = NULL;
		}

		m_vChildren.clear();
	}

	void addChild(T* c)
	{
		m_vChildren.push_back(c);
		c->setParent(this);
	}

	virtual void renderNode(Render2D* renderer, Vector2 vMin, Vector2 vMax, Vector2 vSize)
	{

		renderer->DrawQuad(vMin.X, vMin.Y, vMax.X, vMax.Y, { PIXEL_SOLID, FG_WHITE });

		renderer->DrawString(thingToString<int>(m_vChildren.size()), (vMin + (vSize / 2)).X, (vMin + (vSize / 2)).Y);
	}

	void render(Render2D* renderer, float x, float y)
	{
		float buffer = 0.1;
		Vector2 vSize(1.0, 0.5);
		Vector2 vMin(x, y);
		Vector2 vMax = vMin + vSize;

		float lineStartX = vMax.X - ((vMax - vMin).X / 2);
		float lineStartY = vMax.Y;
		
		renderNode(renderer, vMin, vMax, vSize);

		//	calc x pos for children
		float childX = x; 
		float childY = y + 1;
		float offsetX = 0;

		float leafOffset = 0;
		if (getChildren().size() > 1)
		{
			int totalLeafNodes = getTotalLeafNodes(0, this);
			leafOffset = (((float)totalLeafNodes - 1.0) * 0.5);
		}
		childX = childX - leafOffset;

		for (DataTreeNode* c : m_vChildren)
		{
			float lineEndX = childX + (vSize.X / 2);
			float lineEndY = childY;
			renderer->DrawLine({ lineStartX, lineStartY }, { lineEndX, lineEndY }, { PIXEL_SOLID, FG_LIGHTBLUE });
			
			c->render(renderer, childX, childY);

			childX = childX + vSize.X + (c->getChildren().size() * vSize.X);
		}
	}
};