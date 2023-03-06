class Velocity : 
	public _EntityComponent
{
private:
	Vector2 vVelocity;
	Vector3 vRotationalVelocity;

	float fMaxVelocity;
	float fMass;
	//	other stuff
	bool m_bApplyFriction;

public:
	Velocity(Vector2 velocity, bool friction = true) :
		vVelocity(velocity),
		fMaxVelocity(1.0),
		fMass(1.0),
		m_bApplyFriction(friction),
		_EntityComponent("Physics")	{};

	Velocity(bool friction = true) :
		vVelocity(0, 0),
		fMaxVelocity(1.0),
		fMass(1.0),
		m_bApplyFriction(friction),
		_EntityComponent("Physics")	{};

	void setVelocity(Vector2 vVel) 
	{ 
		vVelocity = vVel; 
	}

	bool applyFriction()
	{
		return m_bApplyFriction;
	}

	Vector2 getVelocity() { return vVelocity; }
};