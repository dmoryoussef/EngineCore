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
		vForce(0, 0),
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
