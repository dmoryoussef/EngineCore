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

	
	Vector3 focus(Vector2 pos, Vector2 screenSize, float z)
	{
		screenSize.div(2);
		Vector2 focus = (screenSize - pos * z);

		float x = focus.X;
		float y = focus.Y;
		return Vector3(x, y, z);
	}


	Vector3 zoom(float z, Vector2 focusScreenPos, Vector3 camPos, Vector2 screenOffset)
	{
		Vector2 vBeforeScaleWorldPosition = WorldPosition(focusScreenPos, camPos.toVec2(), screenOffset, camPos.Z);
		Vector2 vAfterScaleScaleWorldPosition = WorldPosition(focusScreenPos, camPos.toVec2(), screenOffset, z);
		Vector2 Diff = (vBeforeScaleWorldPosition - vAfterScaleScaleWorldPosition) * z;

		return Vector3(camPos.X - Diff.X, camPos.Y - Diff.Y, z );
	}


	Vector2 ScreenPosition(Vector2 WorldPos, Vector2 CameraPos, Vector2 WindowPos)
	{
		return WorldPos + WindowPos + CameraPos;
	}

	Vector2 WorldPosition(Vector2 ScreenPos, Vector2 CameraPos, Vector2 WindowPos, float fScale)
	{
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