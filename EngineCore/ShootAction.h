class ShootAction :
	public _EntityComponent
{
private:
	float fCooldown;
	float fCurrent;
	int nAmmo;
	bool bReady;
	
	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case GAMEPAD_EVENT:
			{
				if (pEvent->get<GamePadEvent>()->getState().XButton)
				{
					if (shoot())
					{
						BaseNode* pParent = getParent();
						addEvent(new ActionEvent(pParent));	
					}
						//	spawn projectile
						
				}
				break;
			}
		}
	}

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

	bool shoot()
	{
		if (bReady && nAmmo > 0)
		{
			nAmmo--;
			fCurrent = fCooldown;
			return true;
		}

		return false;
	}

	void render(Render2D* pRenderer, Vector3 vCameraPosition, Vector2 vWorldMin, Vector2 vWorldMax)
	{
		pRenderer->DrawNum(bReady, 2, 3, FG_WHITE);
	}
};
