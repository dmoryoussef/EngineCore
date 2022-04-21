class Transform2D :
	public _EntityComponent
{
private:
	Vector2 m_vPosition;
	Vector2 m_vScale;
	Vector2 m_vRotation;
	//float angle;

	Vector2 vDeltaTranslate;
	Vector2 vDeltaRotate;

	void onEvent(_Event* pEvent)
	{
		switch (pEvent->m_eType)
		{
			case GAMEPAD_EVENT:
			{
				GamePad Controller = pEvent->get<GamePadEvent>()->getState();
				float LeftStickX = Controller.LeftStickX;
				float LeftStickY = Controller.LeftStickY;
				float RightStickX = Controller.RightStickX;
				float RightStickY = Controller.RightStickY;

				Vector2 vVelocity(LeftStickX, -LeftStickY);

				if (RightStickX != 0.0 || RightStickY != 0.0)
				{
					//	only update rotation of stick is moved
					//	otherwise it resets to 0
					m_vRotation = { -RightStickX, -RightStickY };
					
				}
				
				vVelocity = vVelocity * 0.1;
				m_vPosition = m_vPosition + vVelocity;

				
				break;
			}
		}
	}

public:
	Transform2D(Vector2 P, Vector2 R, Vector2 S) :
		m_vPosition(P),
		m_vScale(S),
		m_vRotation(R),
		_EntityComponent("Transform3D")	
	{
		registerListener(GAMEPAD_EVENT);
	};

	void draw(OutputBuffer *pFrame)
	{
		pFrame->set("TRANSLATION " + m_vPosition.toString(), 2, 2, FG_WHITE + BG_BLACK);
		pFrame->set("ROTATION " + m_vRotation.toString(), 3, 2, FG_WHITE + BG_BLACK);
	}

	void translate(Vector2 translate) {	m_vPosition = m_vPosition + translate;	}

	void setPosition(Vector2 position) { m_vPosition = position; }
	void setRotation(Vector2 rotation) { m_vRotation = rotation; }
	void setScale(Vector2 scale) { m_vScale = scale; }

	Vector2 getForward()
	{
		Vector2 forward(0, 1);
		mat3x3 matRotate = matRotate.Rotate(m_vRotation.getAngle());

		forward = forward * matRotate;

		return forward;
	}


	void render(Render2D* pRenderer, Vector3 vCameraPosition, Vector2 vWorldMin, Vector2 vWorldMax)
	{
		//pRenderer->DrawString(getForward().toString<float>(), 5, 2);
	}

	Vector2 getPosition() { return m_vPosition; }
	Vector2 getRotation() { return m_vRotation; }
	Vector2 getScale() { return m_vScale; }




};