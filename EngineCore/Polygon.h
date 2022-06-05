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

	void createPoly(int sides)
	{
		Verticies.clear();

		for (int nI = 0; nI < sides; nI++)
		{
			Verticies.push_back({ cosf((nI / (float)(sides)) * 2 * PI) , sinf((nI / (float)(sides)) * 2 * PI) });
		}	

		// HACK to rotate points to match object forward vec
		rotate(PI / 2);
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
		mat3x3 matRot = matRot.Rotate(fRad);
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