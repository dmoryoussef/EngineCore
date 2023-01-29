class BuildingTile : public _Tile2D<BuildingTile>
{
private:
	int m_nType;

public:
	BuildingTile() :
		m_nType(0),
		_Tile2D("BUILDING_TILE") {}

	void setType(int t)
	{
		m_nType = t;
	}

	int getType()
	{
		return m_nType;
	}

	bool rayTest()
	{
		return (m_fValue > 0.8);
	}

};

