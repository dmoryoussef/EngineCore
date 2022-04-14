struct Triangle
{
	Vector3 A;
	Vector3 B;
	Vector3 C;

	//Pixel pixel;
	//uint32_t Color;

	Triangle() :
		A({ 0.0, 0.0, 0.0 }),
		B({ 0.0, 0.0, 0.0 }),
		C({ 0.0, 0.0, 0.0 }){};


	Triangle(Vector3 a, Vector3 b, Vector3 c) :
		A(a),
		B(b),
		C(c) {};

	Vector3 normal()
	{
		Vector3 lineA = B - A;
		Vector3 lineB = C - A;

		return crossProduct(lineA, lineB);
	}
};

struct Triangle2D
{
	Vector2 A;
	Vector2 B;
	Vector2 C;

	//Pixel pixel;
	//uint32_t Color;

	Triangle2D() {};

	Triangle2D(Vector2 a, Vector2 b, Vector2 c) :
		A(a),
		B(b),
		C(c) {};

};
