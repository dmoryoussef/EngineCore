#define PI 3.14159f

class Polygon2D
{
private:
	vector<Vector2> Verticies;	// for outlined poly
	vector<Triangle> Tris;	// for filled in poly

	void triangulate()
	{
		//	algorithm to create triangles from the base poly
	}

	

public:
	Polygon2D() {};

	Polygon2D(int sides, float fSize = 1.0)
	{
		createPoly(sides, fSize);
	}

	void createPoly(int sides, float fSize = 1.0)
	{
		Verticies.clear();

		for (int nI = 0; nI < sides; nI++)
		{
			Verticies.push_back({ (cosf((nI / (float)(sides)) * 2 * PI)) * fSize , (sinf((nI / (float)(sides)) * 2 * PI) * fSize) });
		}	

		// HACK to rotate points to match object forward vec
		rotate(PI / 2);
	}

	vector<Vector2> transformedVerts(mat3x3 mWorld)
	{
		vector<Vector2> verts;
		for (auto& vert : Verticies)
		{
			Vector2 v = vert;
			v = v * mWorld;
			verts.push_back(v);
		}
		return verts;
	}

	void transform(Vector2 vScale, float fRad, Vector2 vTranslate)
	{
		scale(vScale);
		rotate(fRad);
		translate(vTranslate);
	}

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
		mat3x3 matRot = matRot.RotateZ(fRad);
		for (auto& vert : Verticies)
		{
			vert = vert * matRot;
		}
	}

	vector<Vector2> getVerticies()
	{
		return Verticies;
	}

};