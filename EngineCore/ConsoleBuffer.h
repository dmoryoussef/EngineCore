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

class ConsoleOutputBuffer : public OutputBuffer
{
private:

public:
	ConsoleOutputBuffer(int width, int height) :
		OutputBuffer(width, height) 
	{	
		m_pBuffer = new Pixel[m_nWidth * m_nHeight];
	}
};