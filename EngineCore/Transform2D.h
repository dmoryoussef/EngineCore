class Transform2D :
	public _EntityComponent
{
private:
	Vector2 m_vPosition;
	Vector2 m_vScale;
	Vector2 m_vRotation;
	float angle;

	Vector2 vDeltaTranslate;
	Vector2 vDeltaRotate;


public:
	Transform2D(Vector2 P, Vector2 R = { 0, 0 }, Vector2 S = { 1, 1 }) :
		m_vPosition(P),
		m_vScale(S),
		m_vRotation(R),
		angle(R.getAngle()),
		_EntityComponent("Transform2D")
	{	};

	void draw(OutputBuffer* pFrame)
	{
		pFrame->set("TRANSLATION " + m_vPosition.toString(), 2, 2, FG_WHITE + BG_BLACK);
		pFrame->set("ROTATION " + m_vRotation.toString(), 3, 2, FG_WHITE + BG_BLACK);
	}

	void translate(Vector2 translate) { m_vPosition = m_vPosition + translate; }
	void scale(Vector2 scale)
	{
		m_vScale.X = m_vScale.X * scale.X;
		m_vScale.Y = m_vScale.Y * scale.Y;
	}


	void addAngle(float theta)
	{
		mat3x3 matRot = matRot.RotateZ(theta);
		Vector2 newRotation = m_vRotation * matRot;
		m_vRotation = newRotation;
	}

	//void rotate(Vector2 rotate)
	//{
	//	mat3x3 matRot = matRot.RotateZ(rotate.getAngle());
	//	m_vRotation = m_vRotation * matRot;
	//}

	void setPosition(Vector2 position) { m_vPosition = position; }
	void setRotation(Vector2 rotation) { m_vRotation = rotation; }
	void setScale(Vector2 scale) { m_vScale = scale; }

	Vector2 getForward()
	{
		return { -m_vRotation.X, -m_vRotation.Y };
	}

	mat3x3 getTransformMatrix()
	{
		mat3x3 mScale = mScale.Scale(m_vScale);
		mat3x3 mRotate = mRotate.RotateZ(m_vRotation.getAngle());
		mat3x3 mTranslate = mTranslate.Translate(m_vPosition);

		mat3x3 mLocal = mLocal.Identity();
		mLocal = mLocal * mScale;
		mLocal = mLocal * mRotate;
		mLocal = mLocal * mTranslate;

		return mLocal;
	}

	string toString()
	{
		return m_vPosition.toString();
	}

	void render(Render2D* pRenderer, Vector3 vCameraPosition, Vector2 vWorldMin, Vector2 vWorldMax)
	{
		//pRenderer->DrawString(getForward().toString<float>(), 5, 2);
	}

	Vector2 getPosition() { return m_vPosition; }
	Vector2 getRotation() { return m_vRotation; }
	Vector2 getScale() { return m_vScale; }

};