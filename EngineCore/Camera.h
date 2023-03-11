class Camera : public _EntityComponent
{
private:
	Vector2 vWorldMin;
	Vector2 vWorldMax;

public:
	Camera() :
		_EntityComponent("Camera")
	{

	}

	Vector2 ScreenPosition(Vector2 WorldPos, Vector2 CameraPos, Vector2 WindowPos)
	{
		return WorldPos + WindowPos + CameraPos;
	}

	Vector2 WorldPosition(Vector2 ScreenPos, Vector2 CameraPos, Vector2 WindowPos, float fScale)
	{
		//Vector2 world = ScreenPos - WindowPos;
		//world = world - CameraPos;
		//world = world + Vector2(1, 1);
		//world = world / fScale;
		//return world;
		return (ScreenPos - WindowPos - CameraPos) / fScale;
	}

	Vector2 WindowPosition(Vector2 ScreenPos, Vector2 WindowPos)
	{
		return ScreenPos - WindowPos;
	}

	Vector2 getWorldMax()
	{
		return vWorldMax;
	}

	Vector2 getWorldMin()
	{
		return vWorldMin;
	}

	void setWorldMinMax(Vector2 min, Vector2 max)
	{
		vWorldMin = min;
		vWorldMax = max;
	}
};