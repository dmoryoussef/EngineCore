class CollisionResponse : public _EntityComponent
{
private:
	//	handle as an event or directly from collision system?
	//		pro of system: faster/same frame
	//		pro of event: decoupled
	// 
	//	from collision system until other objects need to know about collisions
	// 
	//	response types:
	//	onCollision() 
	//	delete
	//	take damage
	//	seperate from overlap
	//	physics
	//		bounce
	//		friction
	//	



public:
	CollisionResponse() :
		_EntityComponent("COLLISION_RESPONSE") {};

	virtual void onCollision() {};
};

class CollisionDamage : public CollisionResponse
{
private:

public:
	CollisionDamage() {};

	void onCollision()
	{

	}
};

class OutOfBoundsCollision : public CollisionResponse
{
private:
	Vector2 vBoundsMin;
	Vector2 vBoundsMax;

public:
	OutOfBoundsCollision(Vector2 min, Vector2 max) :
		vBoundsMin(min),
		vBoundsMax(max) {};

	bool isOutOfBounds(Vector2 pos)
	{
		return isPointvQuad(pos, vBoundsMin, vBoundsMax);
	}

	Vector2 Min()
	{
		return vBoundsMin;
	};
	Vector2 Max()
	{
		return vBoundsMax;
	}

	void onCollision()
	{
		//	out of bounds - delete
		addEvent(new DeleteBaseNodeEvent(getParent()));
	}
};