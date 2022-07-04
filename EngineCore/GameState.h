class GameState : EventListener
{
public:
	GameState() {};

	Vector2 current;
	Vector2 begin; 
	float t;

	void start()
	{
		begin = { 10, 10 };
		current = begin;
		t = 0;
	}

	void render(OutputBuffer *pEngineBuffer)
	{
		Render2D render2d(pEngineBuffer);
		render2d.DrawCircle(current.X, current.Y, 8, { PIXEL_SOLID, FG_WHITE });

	}

	void update(float fDeltaTime)
	{
		if (t < 1)
			t = (t + .0001);

		current = lerp(begin, { 50, 50 }, t);
	}
};