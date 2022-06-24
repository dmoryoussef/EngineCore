class Damage : public _EntityComponent
{
private:
	int m_nCollisionDamage;

public:
	Damage(int nDamage) :
		m_nCollisionDamage(nDamage),
		_EntityComponent("DAMAGE") {};

	int getDamage() 
	{
		return m_nCollisionDamage;
	}
};