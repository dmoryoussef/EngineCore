class Collider : public _EntityComponent
{
private:
	bool m_bColliding;

	//	BaseNode *pCollidedWith;
	//	BaseNode *pIgnoreCollisionWith;

public:
	Collider() :
		m_bColliding(false),
		_EntityComponent("COLLIDER") {};

	bool isColliding() { return m_bColliding; }
	void setColliding(bool b)
	{
		m_bColliding = b;
	}
};