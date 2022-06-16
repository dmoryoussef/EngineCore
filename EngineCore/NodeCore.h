template <typename T>
class _Node
{
protected:
	int m_nUniqueId;
	T* m_pNext;
	T* m_pPrevious;

	void setUniqueId()
	{
		static int m_nNextId = 0;
		m_nUniqueId = ++m_nNextId;
	}

public:
	_Node() :
		m_nUniqueId(0),
		m_pNext(NULL),
		m_pPrevious(NULL)
	{
		//	setUniqueId();
	};

	_Node(int nUniqueId) :
		m_nUniqueId(nUniqueId),
		m_pNext(NULL),
		m_pPrevious(NULL) {};


	void setNext(T* pNext) { m_pNext = pNext; }
	T* getNext() { return m_pNext; }

	template <typename T> T* getNext()
	{
		return static_cast<T*>(m_pNext);
	}
	template <typename T> T* getPrevious()
	{
		return static_cast<T*>(m_pPrevious);
	}

	void setPrevious(T* pPrevious) { m_pPrevious = pPrevious; }
	T* getPrevious() { return m_pPrevious; }

	void setUniqueId(int nUniqueId) { m_nUniqueId = nUniqueId; }
	int getUniqueId() { return m_nUniqueId; }

};

template <typename T>
class _PriorityNode : public _Node<T>
{
protected:
	float m_fPriority;

public:
	_PriorityNode(float fPriority) :
		m_fPriority(fPriority) {}

	float getPriority() { return m_fPriority; }
};


template <typename T> 
class _LinkedList
{
protected:
	int	m_nTotal;

	T* m_pStart;
	T* m_pCurrent;
	T* m_pEnd;

	bool m_bAdvanceBlocked;
	bool m_bIterating;


public:
	_LinkedList() :
		m_nTotal(0),
		m_bAdvanceBlocked(false),
		m_bIterating(false),
		m_pStart(NULL),
		m_pCurrent(NULL),
		m_pEnd(NULL) {};
	virtual ~_LinkedList()
	{
		clearList();
	};

	void setStart(T* pStart) { m_pStart = pStart; }
	void setEnd(T* pEnd) { m_pEnd = pEnd; }
	void addAtEnd(T* pNew)
	{
		if (!m_pStart)
		{
			m_pStart = pNew;
			m_pEnd = pNew;
		}
		else
		{
			pNew->setPrevious(m_pEnd);
			m_pEnd->setNext(pNew);
			m_pEnd = pNew;
		}

		m_nTotal++;
	}
	void addList(T* pStart)
	{
		m_pEnd->setNext(pStart);
	}
	void removeEnd()
	{
		T* pDelete = m_pEnd;

		m_pEnd = m_pEnd->getPrevious();
		m_pEnd->setNext(NULL);

		delete pDelete;

		m_nTotal--;
	}
	void remove(int nIndex)
	{
		T* pRemove = getNodeId(nIndex);
		remove(pRemove);
	}
	void remove(T* pRemove)
	{
		if (pRemove)
		{
			if (pRemove->getPrevious())
				pRemove->getPrevious()->setNext(pRemove->getNext());
			else
				m_pStart = pRemove->getNext();

			if (pRemove->getNext())
				pRemove->getNext()->setPrevious(pRemove->getPrevious());
			else
				m_pEnd = pRemove->getPrevious();


			delete pRemove;
			pRemove = NULL;
			m_nTotal--;
		}
	}
	void clearList()
	{
		T* pCurrent = m_pStart;
		T* pDelete = NULL;
		while (pCurrent != NULL)
		{
			pDelete = pCurrent;
			pCurrent = pCurrent->getNext();
			delete pDelete;
			m_nTotal--;
		}
		delete pCurrent;
		m_pStart = NULL;
		m_pEnd = NULL;
	}
	void setTotal(int nTotal) { m_nTotal = nTotal; }
	int getTotal() { return m_nTotal; }

	template <typename Node> Node* getNode()
	{
		for (T* pCurrent = m_pStart; pCurrent != NULL; pCurrent = pCurrent->getNext())
		{
			if (typeid(Node) == typeid(*pCurrent))
				return static_cast<Node*>(pCurrent);
		}

		return NULL;
	}
	template <typename Node> Node* getNodeId(int nIndex)
	{
		for (T* pCurrent = m_pStart; pCurrent != NULL; pCurrent = pCurrent->getNext())
			if (pCurrent->getUniqueId() == nIndex)
				return static_cast<Node*>(pCurrent);

		return NULL;
	}
	template<typename Type> Type* getCurrent()
	{
		if (Type* pCasted = dynamic_cast<Type*>(m_pCurrent))
			return pCasted;
		else
			return NULL;
	}
	template<typename Type> Type* getStart() { return static_cast<Type*>(m_pStart); }
	template <typename R> void remove()
	{
		remove(getNode<R>());
	}

	T* getNode(T* pNode)
	{
		for (T* pCurrent = m_pStart; pCurrent != NULL; pCurrent = pCurrent->getNext())
		{
			if (pNode == pCurrent)
				return pCurrent;
		}

		return NULL;
	}
	T* getNodeId(int nIndex)
	{
		for (T* pCurrent = m_pStart; pCurrent != NULL; pCurrent = pCurrent->getNext())
			if (pCurrent->getUniqueId() == nIndex)
				return pCurrent;

		return NULL;
	}
	T* getStart() { return m_pStart; }
	T* getCurrent() { return m_pCurrent; }
	T* getEnd() { return m_pEnd; }

	virtual void add(T* pNew)
	{
		if (pNew)
		{
			pNew->setNext(NULL);
			pNew->setPrevious(NULL);
			if (!m_pStart)
			{
				m_pStart = pNew;
				m_pEnd = pNew;
			}
			else
			{
				pNew->setPrevious(NULL);
				pNew->setNext(m_pStart);
				m_pStart->setPrevious(pNew);
				m_pStart = pNew;
			}
			m_nTotal++;
		}
	}
	virtual void pop(T* pPop)
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

			m_nTotal--;
		}
	}

	void removeCurrent()
	{
		if (m_pCurrent)
		{
			if (m_pCurrent->getPrevious())										// if pop has a previous
				m_pCurrent->getPrevious()->setNext(m_pCurrent->getNext());		// then link around pop
			else																// else pop is start
				m_pStart = m_pCurrent->getNext();								// set start to next

			if (m_pCurrent->getNext())											// if pop has a next
				m_pCurrent->getNext()->setPrevious(m_pCurrent->getPrevious());	// then link around
			else																// else pop is end
				m_pEnd = m_pCurrent->getPrevious();								// set end to previous

			T* pDelete = m_pCurrent;
			m_pCurrent = m_pCurrent->getPrevious();
			delete pDelete;
			pDelete = NULL;

			m_nTotal--;
		}
	}
	bool isIterating()
	{
		if (!m_bIterating)
		{
			m_bIterating = true;
			m_pCurrent = m_pStart;
		}
		else
		{
			if (m_pCurrent != NULL)
				m_pCurrent = m_pCurrent->getNext();
		}

		if (m_pCurrent == NULL)
		{
			m_bIterating = false;
		}

		return m_bIterating;
	}
};
