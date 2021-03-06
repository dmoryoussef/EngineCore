class Render : public _EntityComponent
{
private:
	Polygon2D polygon;

public:
	Render(int sides) :
		_EntityComponent("Render") 
	{
		setPoly(sides);
	};

	Polygon2D getPolygon() { return polygon; }

	void setPoly(int sides)
	{
		polygon.createPoly(sides);
	}

	void render(Render2D *pRenderer, Vector3 vCameraPosition, Vector2 vWorldMin, Vector2 vWorldMax)
	{


	}
};