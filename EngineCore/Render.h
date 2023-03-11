class Render : public _EntityComponent
{
private:
	Polygon2D polygon;
	Pixel color;

public:
	Render(int sides) :
		_EntityComponent("Render") 
	{
		setPoly(sides);
	};

	Render(Polygon2D shape, Pixel p = {PIXEL_SOLID, FG_WHITE}) :
		polygon(shape),
		color(p),
		_EntityComponent("Render") {};

	Polygon2D getPolygon() { return polygon; }

	Pixel getColor()
	{
		return color;
	}

	void setPoly(int sides)
	{
		polygon.createPoly(sides);
	}

	void render(Render2D *pRenderer, Vector3 vCameraPosition, Vector2 vWorldMin, Vector2 vWorldMax)
	{


	}
};