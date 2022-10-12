class BuildingTile : public _Tile2D<BuildingTile>
{
public:
	BuildingTile() :
		_Tile2D("BUILDING_TILE") {}

	bool rayTest()
	{
		return (m_fValue > 0.8);
	}

};

