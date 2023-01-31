class BaseTile :
	public BaseNode
{
protected:
	Vector2 m_Position;
	string m_sType;
	bool m_bMouseOver;

public:
	BaseTile(string sType) :
		m_sType(sType),
		m_bMouseOver(false),
		BaseNode() {};

	Vector2 getPosition() { return m_Position; }
	string getType() { return m_sType; }

	bool isMouseOver() { return m_bMouseOver; }
	void setMouseOver(bool mouseOver) { m_bMouseOver = mouseOver; }

	virtual vector<string> toStringStack() 
	{ 
		vector<string> s; 
		return s;
	}

	virtual bool rayTest()
	{
		return false;
	}
};

template <typename TileType> class _Tile2D :
	public BaseTile
{
protected:

	_Tile2D* m_pNorth;
	_Tile2D* m_pSouth;
	_Tile2D* m_pEast;
	_Tile2D* m_pWest;

	bool m_bBlocking;
	
	float m_fValue;
	float m_fMin;
	float m_fMax;

	int m_nTileGroup;
	struct Edges
	{
		int nEdgeID[4];
		bool bWestExists;
		bool bEastExists;
		bool bNorthExists;
		bool bSouthExists;
		Edges()
		{
			reset();
		}

		void reset()
		{
			for (int nI = 0; nI < 4; nI++)
			{
				nEdgeID[nI] = 0;
			}
			bWestExists = false;
			bEastExists = false;
			bNorthExists = false;
			bSouthExists = false;
		}
	} sEdges;


public:
	_Tile2D(string sType) :
		m_fMax(0.0),
		m_fMin(0.0),
		m_fValue(0.0),
		m_pNorth(NULL),
		m_pSouth(NULL),
		m_pEast(NULL),
		m_pWest(NULL),
		m_nTileGroup(0),
		BaseTile(sType) {};



	void initTile(Vector2 Position, _Tile2D* pNorth, _Tile2D* pSouth, _Tile2D* pEast, _Tile2D* pWest)
	{
		m_Position = Position;
		m_pNorth = pNorth;
		m_pSouth = pSouth;
		m_pEast = pEast;
		m_pWest = pWest;

		m_bBlocking = false;

		sEdges.reset();
	}

	Edges& getEdges()
	{
		return sEdges;
	}


	float getValue()
	{
		return m_fValue;
	}

	void setValue(float fValue)
	{
		/*static float fMin = 0;
		static float fMax = 0;

		if (fValue > fMax)
		{
			fMax = fValue;
			m_fMax = fMax;
		}
		if (fValue < fMin)
		{
			fMin = fValue;
			m_fMin = fMin;
		}*/
		m_fValue = fValue;
	}

	void setGroup(int nGroupNumber)
	{
		m_nTileGroup = nGroupNumber;
	}
	int getGroup() { return m_nTileGroup; }

	TileType* getNorth() { return static_cast<TileType*>(m_pNorth); }
	TileType* getSouth() { return static_cast<TileType*>(m_pSouth); }
	TileType* getEast() { return static_cast<TileType*>(m_pEast); }
	TileType* getWest() { return static_cast<TileType*>(m_pWest); }

	void setBlocking(bool blocking)
	{
		m_bBlocking = blocking;
	}

	bool isBlocking()
	{
		return m_bBlocking;
	}

	

};

class DefaultTile :
	public _Tile2D<DefaultTile>
{
private:

public:
	DefaultTile() :
		_Tile2D("DEFAULT_TILE") {};


};
