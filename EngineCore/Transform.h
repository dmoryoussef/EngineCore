class Transform :
	public _EntityComponent
{
private:
	Vector3 vPosition;
	Vector3 vScale;
	Vector3 vRotation;

	Vector3 vDeltaTranslate;
	Vector3 vDeltaRotate;

	

public:
	Transform(Vector3 P, Vector3 R, Vector3 S) :
		vPosition(P),
		vScale(S),
		vRotation(R),
		_EntityComponent("Transform3D")
	{
		
	};


	void update(float fDeltaTime)
	{
		
	}

	void draw(OutputBuffer *pFrame)
	{
		pFrame->set("TRANSLATION " + vPosition.toString(), 2, 2, FG_WHITE + BG_BLACK);
		pFrame->set("ROTATION " + vRotation.toString(), 3, 2, FG_WHITE + BG_BLACK);
	}

	void translate(Vector3 translate)
	{
		vPosition = vPosition + translate;
	}

	void setPosition(Vector3 position) { vPosition = position; }
	void setRotation(Vector3 rotation) { vRotation = rotation; }
	void setScale(Vector3 scale) { vScale = scale; }

	Vector3 getForward()
	{
		Vector3 Forward(0, 0, 1);

	/*	mat4x4 matRotateX = matRotateX.rotateX(vRotation.X);
		mat4x4 matRotateY = matRotateY.rotateY(vRotation.Y);
		mat4x4 matRotateZ = matRotateZ.rotateZ(vRotation.Z);

		Forward = matRotateX * Forward;
		Forward = matRotateY * Forward;
		Forward = matRotateZ * Forward;*/

		return Forward;
	}


	Vector3 getPosition() { return vPosition; }
	Vector3 getRotation() { return vRotation; }
	Vector3 getScale() { return vScale; }

	void render(Render2D* pRenderer, Vector3 vCameraPosition, Vector2 vWorldMin, Vector2 vWorldMax)
	{
		//if (vPosition.toVec2() >= vWorldMin && vPosition.toVec2() <= vWorldMax)
		//{
		//	float fScale = vCameraPosition.Z;
		//	mat2x2 trans = trans.Translate(vPosition.toVec2());
		//	mat2x2 rot = rot.Rotate(0);
		//	mat2x2 scale = scale.Scale({ fScale, fScale });

		//	Triangle2D tri({ 50, 25 }, {25, 60 }, { 70, 40 });

		//	/*tri = tri * trans;*/
		//	//tri = tri * scale;

		//	//	rotate to forward
		//	pRenderer->DrawTriangle(tri.A, tri.B, tri.C, Pixel(PIXEL_SOLID, FG_WHITE));
		//}
	}

};