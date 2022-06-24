class Health : public _EntityComponent
{
private:
	int m_nHealth;

public:
	Health(int nHealth) :
		m_nHealth(nHealth),
		_EntityComponent("HEALTH") {};

	int getHealth()
	{
		return m_nHealth;
	}

	void setHealth(int nHealth)
	{
		m_nHealth = nHealth;
	}
};