class BaseNode :
	public _Node<BaseNode>,
	public _LinkedList<BaseNode>
{
protected:
	BaseNode* m_pParent;
	string m_strName;

public:
	BaseNode() :
		m_strName("BASENODE"),
		m_pParent(NULL),
		_Node()
	{
		setUniqueId();
	};

	BaseNode(int nId) :
		m_strName("BASENODE"),
		m_pParent(NULL),
		_Node(nId) {};

	BaseNode(string strName) :
		m_strName(strName),
		m_pParent(NULL),
		_Node() {};

	virtual ~BaseNode() {};

	string getName()
	{
		return m_strName;
	}

	BaseNode* getParent() { return m_pParent; }
	BaseNode* getRootNode()
	{
		if (m_pParent)
			return m_pParent->getRootNode();
		else
			return this;
	}
	BaseNode* getChild(BaseNode* pChild)
	{
		for (BaseNode* pCurrent = m_pStart; pCurrent != NULL; pCurrent = pCurrent->getNext())
		{
			//	if current Node = searched Node
			if ((pCurrent) == (pChild))
				return (pCurrent);
			else
			{
				//	check child Nodes
				if (BaseNode* pSearch = pCurrent->getChild(pChild))
					return (pSearch);
			}
		}

		return NULL;
	}

	bool isDescendant(BaseNode* pNode)
	{
		if (m_pParent == pNode)
			return true;
		else
		{
			if (pNode->getParent() != NULL)
				isDescendant(pNode->getParent());
			else
				return false;
		}
	}
	bool isAncestor(BaseNode* pNode)
	{
		for (BaseNode* pChild = m_pStart; pChild != NULL; pChild = pChild->getNext())
		{
			if (pChild == pNode)
				return true;
			else
				pChild->isAncestor(pNode);
		}

		return false;
	}

	void deleteSelf()
	{
		m_pParent->remove(this);
	}

	void pop()
	{
		//	link around self
		if (m_pPrevious)									// if pop has a previous
			m_pPrevious->setNext(m_pNext);					// then link around pop
		else
		{

			if (m_pParent)
				m_pParent->setStart(m_pNext);
		}

		if (m_pNext)										// if pop has a next
			m_pNext->setPrevious(m_pPrevious);				// then link around
		else
		{

			if (m_pParent)
				m_pParent->setEnd(m_pPrevious);				// else pop is end
		}

		//	update total
		if (m_pParent)
			m_pParent->setTotal(m_pParent->getTotal() - 1);


		//	unlink self
		m_pPrevious = NULL;
		m_pNext = NULL;
		m_pParent = NULL;
	}
	void setName(string strName)
	{
		m_strName = strName;
	}
	void popChild(BaseNode* pPop)
	{
		if (pPop)
		{
			if (pPop->getPrevious())									// if pop has a previous
				pPop->getPrevious()->setNext(pPop->getNext());			// then link around pop
			else														// else pop is start
				m_pStart = pPop->getNext();								// set start to next

			if (pPop->getNext())										// if pop has a next
				pPop->getNext()->setPrevious(pPop->getPrevious());		// then link around
			else														// else pop is end
				m_pEnd = pPop->getPrevious();							// set end to previous

			pPop->setPrevious(NULL);
			pPop->setNext(NULL);
			pPop->setParent(NULL);

			m_nTotal--;
		}
	}
	void setParent(BaseNode* pParent)
	{
		//	if (pParent)
		//		notify parent this child is linking
		//		m_pParent->childIsLinking(this);
		//	else
		//		this child is switching to NULL
		//		if m_pParent
		//			notify parent this child is UNLINKING
		//			m_pParent->childIsUnLinking(this);
		m_pParent = pParent;
	}
	void addSibling(BaseNode* pSibling)
	{
		m_pParent->addChild(pSibling);
	}

	template <typename Node> Node* getChild()
	{
		for (BaseNode* pCurrent = m_pStart; pCurrent != NULL; pCurrent = pCurrent->getNext())
		{
			//	if current Node = searched Node
			if (typeid(*pCurrent) == typeid(Node))
				return dynamic_cast<Node*>(pCurrent);
		}
		return NULL;
	}
	template <typename Node> Node* getDescendant()
	{
		for (BaseNode* pCurrent = m_pStart; pCurrent != NULL; pCurrent = pCurrent->getNext())
		{
			//	if current Node = searched Node
			if (typeid(*pCurrent) == typeid(Node))
				return dynamic_cast<Node*>(pCurrent);
			else
			{
				//	check child Nodes
				if (Node* pSearch = pCurrent->getChild<Node>())
					return (pSearch);
			}
		}

		return NULL;
	}
	template <typename T> T* getParent() { return static_cast<T*>(m_pParent); }

	virtual void addChild(BaseNode* pChild)
	{
		if (pChild)
		{
			pChild->setParent(this);
			addAtEnd(pChild);
		}
	}

	virtual void start() {}

	virtual void update(float fDeltaTime) {}

	virtual void rootUpdate(float fDeltaTime)
	{
		update(fDeltaTime);
		while (isIterating())
			getCurrent()->rootUpdate(fDeltaTime);
	}
	virtual string toString() 
	{
		return "";
	}

	void baseUpdate(float fDeltaTime)
	{
		update(fDeltaTime);
		while (isIterating())
		{
			getCurrent()->baseUpdate(fDeltaTime);
		}
	}

	void baseRender(Render2D *pRenderer, Vector3 vCameraPosition, Vector2 vWorldMin, Vector2 vWorldMax)
	{
		render(pRenderer, vCameraPosition, vWorldMin, vWorldMax);
		while (isIterating())
		{
			getCurrent()->baseRender(pRenderer, vCameraPosition, vWorldMin, vWorldMax);
		}
	}

	virtual void render(Render2D *pRenderer, Vector3 vCameraPosition, Vector2 vWorldMin, Vector2 vWorldMax) {};
};