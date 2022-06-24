class Physics : 
	public _EntityComponent
{
private:
	Vector2 vVelocity;
	Vector3 vRotationalVelocity;

	float fMaxVelocity;
	float fMass;
	//	other stuff


public:
	Physics(Vector2 velocity) :
		vVelocity(velocity),
		fMaxVelocity(0.02),
		fMass(1.0),
		_EntityComponent("Physics")
	{

	};

	Physics() :
		vVelocity({0, 0}),
		fMaxVelocity(0.02),
		fMass(1.0),
		_EntityComponent("Physics")
	{

	};

	void setVelocity(Vector2 vVel) 
	{ 
		if (vVelocity.magnitude() < fMaxVelocity && 
			vVel.magnitude() < fMaxVelocity)
			vVelocity = vVel; 
	}
	Vector2 getVelocity() { return vVelocity; }
};