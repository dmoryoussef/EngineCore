class ShootAction :
	public _EntityComponent
{
private:
	float fCooldown;
	float fCurrent;
	int nAmmo;
	bool bReady;

public:
	ShootAction(int ammo, float cooldown) :
		fCooldown(cooldown),
		fCurrent(0),
		bReady(true),
		nAmmo(ammo),
		_EntityComponent("ShootAction") 
	{
		registerListener(GAMEPAD_EVENT);
	};

	void update(float fDeltaTime)
	{
		if (fCurrent > 0)
		{
			bReady = false;
			fCurrent = fCurrent - fDeltaTime;
		}
		else
		{
			bReady = true;
			fCurrent = 0;
		}

	}

	bool canShoot()
	{
		if (bReady && nAmmo > 0)
		{
			nAmmo--;
			fCurrent = fCooldown;
			return true;
		}

		return false;
	}


};
