class Accelerate : public _EntityComponent
{
private:
	float m_fMax;
	float m_fCurrent;
	Vector2 vForce;

public:
	Accelerate(float max) :
		m_fMax(max),
		m_fCurrent(0.0),
		vForce(),
		_EntityComponent("Thrust") {};

	float getMax() { return m_fMax; }

	Vector2 getForce()
	{
		return vForce;
	}
	void setForce(Vector2 force)
	{
		vForce = force;
	}
};

class Velocity : 
	public _EntityComponent
{
private:
	Vector2 vVelocity;
	Vector3 vRotationalVelocity;

	float fMaxVelocity;
	float fMass;
	//	other stuff


public:
	Velocity(Vector2 velocity) :
		vVelocity(velocity),
		fMaxVelocity(1.0),
		fMass(1.0),
		_EntityComponent("Physics")	{};

	Velocity() :
		vVelocity({0, 0}),
		fMaxVelocity(1.0),
		fMass(1.0),
		_EntityComponent("Physics")
	{

	};

	void setVelocity(Vector2 vVel) 
	{ 
		vVelocity = vVel; 
	}

	Vector2 getVelocity() { return vVelocity; }
};