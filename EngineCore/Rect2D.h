class Rect2D
{
private:
	Vector2 vMin;
	Vector2 vMax;

	Rect2D overLap(Rect2D A, Rect2D B)
	{
		Vector2 min = Vector2(max(A.getMin().X, B.getMin().X), max(A.getMin().Y, B.getMin().Y));
		Vector2 max = Vector2(min(A.getMax().X, B.getMax().X), min(A.getMax().Y, B.getMax().Y));

		Rect2D C(min, max);

		return C;
	}

public:
	Rect2D(Vector2 min, Vector2 max) :
		vMin(min),
		vMax(max) {};

	Rect2D(Rect2D A, Rect2D B)
	{
		Rect2D overlap = overLap(A, B);
		vMin = overlap.getMin();
		vMax = overlap.getMax();
	}

	Vector2 getSize()
	{
		return vMax - vMin;
	}

	Vector2 getMin()
	{
		return vMin;
	}

	Vector2 getMax()
	{
		return vMax;
	}

	float getPerimeter()
	{
		return 0;
	}

	float getArea()
	{
		return 0;
	}

	void render(Render2D* r, Pixel c)
	{
		r->DrawLine(vMin, { vMax.X, vMin.Y }, c);
		r->DrawLine(vMin, { vMin.X, vMax.Y }, c);
		r->DrawLine({ vMax.X, vMin.Y }, vMax, c);
		r->DrawLine({ vMin.X, vMax.Y }, vMax, c);
	}
};