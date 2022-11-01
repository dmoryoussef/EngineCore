bool isPointvCircle(Vector2 p, Vector2 cir, float rad, float er = 0)
{
	return (hypotenuse(p, cir) <= rad + er);
}

bool isPointvQuad(Vector2 p, Vector2 min, Vector2 max)
{
	return (min <= p && max >= p);
}

bool isPointvLine(Vector2 p, Vector2 a, Vector2 b, float er = 0)
{
	float pa = hypotenuse(a, p);
	float pb = hypotenuse(b, p); 
	return ((pa + pb) - hypotenuse(a, b) <= er);
}

bool isLinevLine()
{
	return false;
}

bool isQuadvQuad(Vector2 minA, Vector2 maxA, Vector2 minB, Vector2 maxB)
{
	return (isPointvQuad(minA, minB, maxB) || isPointvQuad(maxA, minB, maxB) || isPointvQuad({ maxA.X, minA.Y }, minB, maxB) || isPointvQuad({ minA.X, maxA.Y }, minB, maxB));
}