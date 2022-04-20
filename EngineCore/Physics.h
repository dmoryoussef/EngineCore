class Physics : 
	public _EntityComponent
{
private:
	Vector2 vVelocity;
	Vector3 vRotationalVelocity;

	float fMass;
	//	other stuff

	

public:
	Physics(float velocity) :
		vVelocity(velocity),
		fMass(1.0),
		_EntityComponent("Physics")
	{

	};

	Physics(Vector2 velocity) :
		vVelocity(velocity),
		fMass(1.0),
		_EntityComponent("Physics")
	{

	};

	Physics() :
		vVelocity({0, 0}),
		fMass(1.0),
		_EntityComponent("Physics")
	{

	};

	Vector2 getVelocity() { return vVelocity; }
};