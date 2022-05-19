class Polygon : _EntityComponent
{
private:
	vector<Vector2> Verticies;	// for outlined poly
	vector<Triangle> Tris;	// for filled in poly

	void triangulate()
	{
		//	algorithm to create triangles from the base poly
	}

	void createPoly(int verts)
	{
		Verticies.push_back({ 0.0, 0.0 });
		for (int nI = 0; nI < verts; nI++)
		{
			Verticies.push_back({ cosf(nI / (float)(verts - 1) * 2.0f * 3.14159f) , sinf(nI / (float)(verts - 1) * 2.0f * 3.14159f) });
		}	
	}

public:
	Polygon() :
	_EntityComponent("POLYGON") {};

	void scale(Vector2 scale)
	{
		mat3x3 matScale = matScale.Scale(scale);
		for (auto& vert : Verticies)
		{
			vert = vert * matScale;
		}
	}

	void translate(Vector2 transl)
	{
		mat3x3 matTrans = matTrans.Translate(transl);
		for (auto& vert : Verticies)
		{
			vert = vert * matTrans;
		}
	}

	void rotate(float fRad)
	{
		mat3x3 matRot = matRot.Rotate(fRad);
		for (auto& vert : Verticies)
		{
			vert = vert * matRot;
		}
	}


};