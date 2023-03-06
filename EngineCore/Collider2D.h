class Collider2D : public _EntityComponent
{
private:
	bool m_bColliding;
	Polygon2D PolygonCollider;

	//	BaseNode *pCollidedWith;
	//	BaseNode *pIgnoreCollisionWith;

public:
	Collider2D() :
		m_bColliding(false),
		_EntityComponent("COLLIDER") {};

	Collider2D(Polygon2D poly) :
		m_bColliding(false),
		PolygonCollider(poly),
		_EntityComponent("COLLIDER") {};

	Polygon2D getPoly()
	{
		return PolygonCollider;
	}

	bool isColliding() { return m_bColliding; }
	void setColliding(bool b)
	{
		m_bColliding = b;
	}
};