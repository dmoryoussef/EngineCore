class Transform3D :
	public _EntityComponent
{
private:
	Vector3 vPosition;
	Vector3 vScale;
	Vector3 vRotation;

	Vector3 vDeltaTranslate;
	Vector3 vDeltaRotate;



public:
	Transform3D(Vector3 P, Vector3 R, Vector3 S) :
		vPosition(P),
		vScale(S),
		vRotation(R),
		_EntityComponent("Transform3D") {};

	void draw(OutputBuffer* pFrame)
	{
		pFrame->set("TRANSLATION " + vPosition.toString(), 2, 2, FG_WHITE + BG_BLACK);
		pFrame->set("ROTATION " + vRotation.toString(), 3, 2, FG_WHITE + BG_BLACK);
	}

	void setPosition(Vector3 position) { vPosition = position; }
	void setRotation(Vector3 rotation) { vRotation = rotation; }
	void setScale(Vector3 scale) { vScale = scale; }



	Vector3 getPosition() { return vPosition; }
	Vector3 getRotation() { return vRotation; }
	Vector3 getScale() { return vScale; }

};