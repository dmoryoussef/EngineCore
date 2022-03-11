class Color
{
private:
	struct consoleColor
	{
		int FG_Color;
		int BG_Color;
		int Pixel;

		consoleColor(int fg = 0, int bg = 0, int p = 0)
		{
			FG_Color = fg;
			BG_Color = bg;
			Pixel = p;
		}

		int color()
		{
			return FG_Color + BG_Color;
		}
	} console;

	struct rgbColor
	{
		int Red;
		int Green;
		int Blue;

		rgbColor(int r = 0, int g = 0, int b = 0)
		{
			Red = r;
			Green = g;
			Blue = b;
		}

	} rgb;

public:
	Color(int fg, int bg) :
		console(fg, bg)
	{}

};