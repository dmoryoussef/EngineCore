class Physics : 
	public _EntityComponent
{
private:
	Vector3 vVelocity;

public:
	Physics() :
		_EntityComponent("Velocity3D") {};

	Vector3 getVelocity() { return vVelocity; }
};