const int BG_BLACK = 0;
const int BG_DARKBLUE = 16;
const int BG_DARKGREEN = 32;
const int BG_DARKTEAL = 48;
const int BG_DARKRED = 64;
const int BG_PURPLE = 80;
const int BG_DARKYELLOW = 96;
const int BG_LIGHTGREY = 112;
const int BG_DARKGREY = 128;
const int BG_LIGHTBLUE = 144;
const int BG_LIGHTGREEN = 160;
const int BG_LIGHTTEAL = 176;
const int BG_LIGHTRED = 192;
const int BG_PINK = 208;
const int BG_YELLOW = 224;
const int BG_WHITE = 240;

const int FG_BLACK = 0;
const int FG_DARKBLUE = 1;
const int FG_DARKGREEN = 2;
const int FG_DARKTEAL = 3;
const int FG_DARKRED = 4;
const int FG_PURPLE = 5;
const int FG_DARKYELLOW = 6;
const int FG_LIGHTGREY = 7;
const int FG_DARKGREY = 8;
const int FG_LIGHTBLUE = 9;
const int FG_LIGHTGREEN = 10;
const int FG_LIGHTTEAL = 11;
const int FG_LIGHTRED = 12;
const int FG_PINK = 13;
const int FG_YELLOW = 14;
const int FG_WHITE = 15;

const int PIXEL_QUARTER = 176;
const int PIXEL_HALF = 177;
const int PIXEL_THREEQUARTER = 178;
const int PIXEL_SOLID = 219;

class Render2D
{
protected:
	OutputBuffer *m_pTargetBuffer;
	int nHeight;
	int nWidth;

public:
	Render2D() {}

	Render2D(OutputBuffer *pBuffer) :
		m_pTargetBuffer(pBuffer),
		nHeight(pBuffer->getHeight()),
		nWidth(pBuffer->getWidth()) {};

	Vector2 getSize()
	{
		return Vector2(nWidth, nHeight);
	}

	Pixel getColorscaleColor(float fValue)
	{
		fValue /= 100;
		int BG_Color = 0;
		int FG_Color = 0;
		char sym = 0;
		//	convert float range of { 0.0 , 1.0 }
		//	to int value of range { 1, 13 }
		int nValue = fValue * 12;
		switch (nValue)
		{
		case 0: BG_Color = BG_LIGHTBLUE; FG_Color = FG_LIGHTBLUE; sym = PIXEL_SOLID;
			break;
		case 1: BG_Color = BG_DARKBLUE; FG_Color = FG_LIGHTBLUE; sym = PIXEL_QUARTER;
			break;
		case 2: BG_Color = BG_DARKBLUE; FG_Color = FG_DARKBLUE; sym = PIXEL_HALF;
			break;
		case 3:  BG_Color = BG_DARKBLUE; FG_Color = FG_DARKBLUE; sym = PIXEL_THREEQUARTER;
			break;
		case 4: BG_Color = BG_DARKBLUE; FG_Color = FG_DARKBLUE; sym = PIXEL_SOLID;
			break;
		case 5: BG_Color = BG_DARKBLUE; FG_Color = FG_LIGHTBLUE; sym = PIXEL_QUARTER;
			break;
		case 6: BG_Color = BG_DARKBLUE; FG_Color = FG_LIGHTBLUE; sym = PIXEL_HALF;
			break;
		case 7: BG_Color = BG_DARKBLUE; FG_Color = FG_LIGHTBLUE; sym = PIXEL_THREEQUARTER;
			break;
		case 8: BG_Color = BG_DARKBLUE; FG_Color = FG_LIGHTBLUE; sym = PIXEL_SOLID;
			break;
		case 9: BG_Color = BG_DARKBLUE; FG_Color = FG_DARKBLUE; sym = PIXEL_QUARTER;
			break;
		case 10: BG_Color = BG_DARKBLUE; FG_Color = FG_DARKBLUE; sym = PIXEL_HALF;
			break;
		case 11: BG_Color = BG_DARKBLUE; FG_Color = FG_DARKBLUE; sym = PIXEL_THREEQUARTER;
			break;
		case 12: BG_Color = BG_DARKBLUE; FG_Color = FG_DARKBLUE; sym = PIXEL_SOLID;
			break;
		}

		return Pixel(sym, BG_Color + FG_Color);
	}

	Pixel getGreyscaleColor(float fValue)
	{
		int BG_Color = 0;
		int FG_Color = 0;
		int sym = 0;
		//	convert float range of { 0.0 , 1.0 }
		//	to int value of range { 1, 13 }
		int nValue = fValue * 12;

		switch (nValue)
		{
		case 0: BG_Color = BG_BLACK; FG_Color = FG_BLACK; sym = PIXEL_SOLID;
			break;
		case 1: BG_Color = BG_BLACK; FG_Color = FG_DARKGREY; sym = PIXEL_QUARTER;
			break;
		case 2: BG_Color = BG_BLACK; FG_Color = FG_DARKGREY; sym = PIXEL_HALF;
			break;
		case 3: BG_Color = BG_BLACK; FG_Color = FG_DARKGREY; sym = PIXEL_THREEQUARTER;
			break;
		case 4: BG_Color = BG_BLACK; FG_Color = FG_DARKGREY; sym = PIXEL_SOLID;
			break;
		case 5: BG_Color = BG_DARKGREY; FG_Color = FG_LIGHTGREY; sym = PIXEL_QUARTER;
			break;
		case 6: BG_Color = BG_DARKGREY; FG_Color = FG_LIGHTGREY; sym = PIXEL_HALF;
			break;
		case 7: BG_Color = BG_DARKGREY; FG_Color = FG_LIGHTGREY; sym = PIXEL_THREEQUARTER;
			break;
		case 8: BG_Color = BG_DARKGREY; FG_Color = FG_LIGHTGREY; sym = PIXEL_SOLID;
			break;
		case 9:  BG_Color = BG_LIGHTGREY; FG_Color = FG_WHITE; sym = PIXEL_QUARTER;
			break;
		case 10: BG_Color = BG_LIGHTGREY; FG_Color = FG_WHITE; sym = PIXEL_HALF;
			break;
		case 11: BG_Color = BG_LIGHTGREY; FG_Color = FG_WHITE; sym = PIXEL_THREEQUARTER;
			break;
		case 12: BG_Color = BG_LIGHTGREY; FG_Color = FG_WHITE; sym = PIXEL_SOLID;
			break;
		default: BG_Color = BG_BLACK; FG_Color = FG_BLACK; sym = PIXEL_SOLID;

		}

		return Pixel(sym, BG_Color + FG_Color);
	}

	inline void DrawPoint(int nX, int nY, Pixel pixel)
	{
		if (nY >= 0 && nX >= 0 && nY < nHeight && nX < nWidth)
			m_pTargetBuffer->set(pixel, nX, nY);
	}

	void DrawLine(Vector2 Start, Vector2 End, Pixel pixel)
	{
		int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
		dx = End.X - Start.X;
		dy = End.Y - Start.Y;
		dx1 = abs(dx);
		dy1 = abs(dy);
		px = 2 * dy1 - dx1;
		py = 2 * dx1 - dy1;

		if (dy1 <= dx1)
		{
			if (dx >= 0)
			{
				x = Start.X;
				y = Start.Y;
				xe = End.X;
			}
			else
			{
				x = End.X;
				y = End.Y;
				xe = Start.X;
			}
			DrawPoint(x, y, pixel);

			for (i = 0; x < xe; i++)
			{
				x = x + 1;
				if (px < 0)
					px = px + 2 * dy1;

				else
				{
					if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1; else y = y - 1;
					px = px + 2 * (dy1 - dx1);
				}
				DrawPoint(x, y, pixel);
			}
		}

		else
		{
			if (dy >= 0)
			{
				x = Start.X;
				y = Start.Y;
				ye = End.Y;
			}
			else
			{
				x = End.X;
				y = End.Y;
				ye = Start.Y;
			}
			DrawPoint(x, y, pixel);

			for (i = 0; y < ye; i++)
			{
				y = y + 1;
				if (py <= 0)
					py = py + 2 * dx1;
				else
				{
					if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1; else x = x - 1;
					py = py + 2 * (dx1 - dy1);

				}
				DrawPoint(x, y, pixel);
			}
		}

	}
	void DrawTriangle(Vector2 a, Vector2 b, Vector2 c, Pixel pixel)
	{
		DrawLine(a, b, pixel);
		DrawLine(b, c, pixel);
		DrawLine(c, a, pixel);
	}
	void DrawPoly(vector<Vector2> verticies, Pixel pixel)
	{
		for (int i = 0; i < verticies.size() - 1; i++)
		{
			DrawLine(verticies[i], verticies[i + 1], pixel);
		}

		DrawLine(verticies[verticies.size()], verticies[0], pixel);
	}

	//void DrawTriangle(Triangle tri, Pixel pix)
	//{
	//	DrawTriangle(tri.A.toVec2(), tri.B.toVec2(), tri.C.toVec2(), pix);
	//}

	//void FillTriangle(Triangle triangle, Pixel pixel)
	//{
	//	int x1 = triangle.A.X;
	//	int y1 = triangle.A.Y;

	//	int x2 = triangle.B.X;
	//	int y2 = triangle.B.Y;

	//	int x3 = triangle.C.X;
	//	int y3 = triangle.C.Y;

	//	auto SWAP = [](int& x, int& y) { int t = x; x = y; y = t; };

	//	auto drawline = [&](int sx, int ex, int ny)
	//	{
	//		for (int i = sx; i <= ex; i++)
	//			m_pTargetBuffer->set(pixel.m_chChar, i, ny, pixel.m_nColor);
	//	};



	//	int t1x, t2x, y, minx, maxx, t1xp, t2xp;

	//	bool changed1 = false;

	//	bool changed2 = false;

	//	int signx1, signx2, dx1, dy1, dx2, dy2;

	//	int e1, e2;

	//	// Sort vertices

	//	if (y1 > y2) { SWAP(y1, y2); SWAP(x1, x2); }

	//	if (y1 > y3) { SWAP(y1, y3); SWAP(x1, x3); }

	//	if (y2 > y3) { SWAP(y2, y3); SWAP(x2, x3); }



	//	t1x = t2x = x1; y = y1;   // Starting points

	//	dx1 = (int)(x2 - x1); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }

	//	else signx1 = 1;

	//	dy1 = (int)(y2 - y1);



	//	dx2 = (int)(x3 - x1); if (dx2 < 0) { dx2 = -dx2; signx2 = -1; }

	//	else signx2 = 1;

	//	dy2 = (int)(y3 - y1);



	//	if (dy1 > dx1) {   // swap values

	//		SWAP(dx1, dy1);

	//		changed1 = true;

	//	}

	//	if (dy2 > dx2) {   // swap values

	//		SWAP(dy2, dx2);

	//		changed2 = true;

	//	}



	//	e2 = (int)(dx2 >> 1);

	//	// Flat top, just process the second half

	//	if (y1 == y2) goto next;

	//	e1 = (int)(dx1 >> 1);



	//	for (int i = 0; i < dx1;) {

	//		t1xp = 0; t2xp = 0;

	//		if (t1x < t2x) { minx = t1x; maxx = t2x; }

	//		else { minx = t2x; maxx = t1x; }

	//		// process first line until y value is about to change

	//		while (i < dx1) {

	//			i++;

	//			e1 += dy1;

	//			while (e1 >= dx1) {

	//				e1 -= dx1;

	//				if (changed1) t1xp = signx1;//t1x += signx1;

	//				else          goto next1;

	//			}

	//			if (changed1) break;

	//			else t1x += signx1;

	//		}

	//		// Move line

	//	next1:

	//		// process second line until y value is about to change

	//		while (1) {

	//			e2 += dy2;

	//			while (e2 >= dx2) {

	//				e2 -= dx2;

	//				if (changed2) t2xp = signx2;//t2x += signx2;

	//				else          goto next2;

	//			}

	//			if (changed2)     break;

	//			else              t2x += signx2;

	//		}

	//	next2:

	//		if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;

	//		if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;

	//		drawline(minx, maxx, y);    // Draw line from min to max points found on the y

	//									 // Now increase y

	//		if (!changed1) t1x += signx1;

	//		t1x += t1xp;

	//		if (!changed2) t2x += signx2;

	//		t2x += t2xp;

	//		y += 1;

	//		if (y == y2) break;



	//	}

	//next:

	//	// Second half

	//	dx1 = (int)(x3 - x2); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }

	//	else signx1 = 1;

	//	dy1 = (int)(y3 - y2);

	//	t1x = x2;



	//	if (dy1 > dx1) {   // swap values

	//		SWAP(dy1, dx1);

	//		changed1 = true;

	//	}

	//	else changed1 = false;



	//	e1 = (int)(dx1 >> 1);



	//	for (int i = 0; i <= dx1; i++) {

	//		t1xp = 0; t2xp = 0;

	//		if (t1x < t2x) { minx = t1x; maxx = t2x; }

	//		else { minx = t2x; maxx = t1x; }

	//		// process first line until y value is about to change

	//		while (i < dx1) {

	//			e1 += dy1;

	//			while (e1 >= dx1) {

	//				e1 -= dx1;

	//				if (changed1) { t1xp = signx1; break; }//t1x += signx1;

	//				else          goto next3;

	//			}

	//			if (changed1) break;

	//			else   	   	  t1x += signx1;

	//			if (i < dx1) i++;

	//		}

	//	next3:

	//		// process second line until y value is about to change

	//		while (t2x != x3) {

	//			e2 += dy2;

	//			while (e2 >= dx2) {

	//				e2 -= dx2;

	//				if (changed2) t2xp = signx2;

	//				else          goto next4;

	//			}

	//			if (changed2)     break;

	//			else              t2x += signx2;

	//		}

	//	next4:



	//		if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;

	//		if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;

	//		drawline(minx, maxx, y);

	//		if (!changed1) t1x += signx1;

	//		t1x += t1xp;

	//		if (!changed2) t2x += signx2;

	//		t2x += t2xp;

	//		y += 1;

	//		if (y > y3) return;

	//	}

	//}

	void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, Pixel pixel)

	{

		auto SWAP = [](int& x, int& y) { int t = x; x = y; y = t; };

		auto drawline = [&](int sx, int ex, int ny)
		{
			for (int i = sx; i <= ex; i++)
				m_pTargetBuffer->set(pixel.m_chChar, i, ny, pixel.m_nColor);
		};



		int t1x, t2x, y, minx, maxx, t1xp, t2xp;

		bool changed1 = false;

		bool changed2 = false;

		int signx1, signx2, dx1, dy1, dx2, dy2;

		int e1, e2;

		// Sort vertices

		if (y1 > y2) { SWAP(y1, y2); SWAP(x1, x2); }

		if (y1 > y3) { SWAP(y1, y3); SWAP(x1, x3); }

		if (y2 > y3) { SWAP(y2, y3); SWAP(x2, x3); }



		t1x = t2x = x1; y = y1;   // Starting points

		dx1 = (int)(x2 - x1); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }

		else signx1 = 1;

		dy1 = (int)(y2 - y1);



		dx2 = (int)(x3 - x1); if (dx2 < 0) { dx2 = -dx2; signx2 = -1; }

		else signx2 = 1;

		dy2 = (int)(y3 - y1);



		if (dy1 > dx1) {   // swap values

			SWAP(dx1, dy1);

			changed1 = true;

		}

		if (dy2 > dx2) {   // swap values

			SWAP(dy2, dx2);

			changed2 = true;

		}



		e2 = (int)(dx2 >> 1);

		// Flat top, just process the second half

		if (y1 == y2) goto next;

		e1 = (int)(dx1 >> 1);



		for (int i = 0; i < dx1;) {

			t1xp = 0; t2xp = 0;

			if (t1x < t2x) { minx = t1x; maxx = t2x; }

			else { minx = t2x; maxx = t1x; }

			// process first line until y value is about to change

			while (i < dx1) {

				i++;

				e1 += dy1;

				while (e1 >= dx1) {

					e1 -= dx1;

					if (changed1) t1xp = signx1;//t1x += signx1;

					else          goto next1;

				}

				if (changed1) break;

				else t1x += signx1;

			}

			// Move line

		next1:

			// process second line until y value is about to change

			while (1) {

				e2 += dy2;

				while (e2 >= dx2) {

					e2 -= dx2;

					if (changed2) t2xp = signx2;//t2x += signx2;

					else          goto next2;

				}

				if (changed2)     break;

				else              t2x += signx2;

			}

		next2:

			if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;

			if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;

			drawline(minx, maxx, y);    // Draw line from min to max points found on the y

										 // Now increase y

			if (!changed1) t1x += signx1;

			t1x += t1xp;

			if (!changed2) t2x += signx2;

			t2x += t2xp;

			y += 1;

			if (y == y2) break;



		}

	next:

		// Second half

		dx1 = (int)(x3 - x2); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }

		else signx1 = 1;

		dy1 = (int)(y3 - y2);

		t1x = x2;



		if (dy1 > dx1) {   // swap values

			SWAP(dy1, dx1);

			changed1 = true;

		}

		else changed1 = false;



		e1 = (int)(dx1 >> 1);



		for (int i = 0; i <= dx1; i++) {

			t1xp = 0; t2xp = 0;

			if (t1x < t2x) { minx = t1x; maxx = t2x; }

			else { minx = t2x; maxx = t1x; }

			// process first line until y value is about to change

			while (i < dx1) {

				e1 += dy1;

				while (e1 >= dx1) {

					e1 -= dx1;

					if (changed1) { t1xp = signx1; break; }//t1x += signx1;

					else          goto next3;

				}

				if (changed1) break;

				else   	   	  t1x += signx1;

				if (i < dx1) i++;

			}

		next3:

			// process second line until y value is about to change

			while (t2x != x3) {

				e2 += dy2;

				while (e2 >= dx2) {

					e2 -= dx2;

					if (changed2) t2xp = signx2;

					else          goto next4;

				}

				if (changed2)     break;

				else              t2x += signx2;

			}

		next4:

			if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;

			if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;

			drawline(minx, maxx, y);

			if (!changed1) t1x += signx1;

			t1x += t1xp;

			if (!changed2) t2x += signx2;

			t2x += t2xp;

			y += 1;

			if (y > y3) return;

		}
	}

	void DrawQuad(int nMinX, int nMinY, int nMaxX, int nMaxY, Pixel pixel)
	{
		if (nMinY < 0)
			nMinY = 0;
		if (nMinX < 0)
			nMinX = 0;
		if (nMaxX > nWidth)
			nMaxX = nWidth;
		if (nMaxY > nHeight)
			nMaxY = nHeight;

		for (int nY = nMinY; nY < nMaxY; nY++)
		{
			for (int nX = nMinX; nX < nMaxX; nX++)
			{
				DrawPoint(nX, nY, pixel);
			}
		}
	}

	template <typename T>
	void DrawNum(T d, int nX, int nY, int color)
	{
		if (nX >= 0 && nY >= 0)
			if (nX < nWidth && nY < nHeight)
				m_pTargetBuffer->set(to_string(d), nX, nY, color);
	}

	void DrawChar(char ch, int nX, int nY, int color)
	{
		if (nX >= 0 && nY >= 0)
			if (nX < nWidth && nY < nHeight)
				m_pTargetBuffer->set(ch, nX, nY, color);
	}

	void DrawString(string str, int x, int y)
	{
		for (int nI = 0; nI < str.length(); nI++)
		{
			m_pTargetBuffer->set(str, x, y, FG_WHITE);
		}
	}

	virtual void init(OutputBuffer* pBuffer)
	{
		m_pTargetBuffer = pBuffer;
		nHeight = pBuffer->getHeight();
		nWidth = pBuffer->getWidth();
	}

	//virtual void draw(BaseNode* pGameData, BaseNode* pCamera) {}


};