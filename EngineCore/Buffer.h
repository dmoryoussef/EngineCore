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

class Pixel
{
public:
	int m_nColor;
	int m_chChar;

	Pixel() :
		m_chChar(0),
		m_nColor(0) {}

	Pixel(int nChar, int nColor) :
		m_chChar(nChar),
		m_nColor(nColor) {}

	void set(Pixel pixel)
	{
		m_nColor = pixel.m_nColor;
		m_chChar = pixel.m_chChar;
	}

	void set(int chChar, int nColor)
	{
		m_nColor = nColor;
		m_chChar = chChar;
	}
};

class Buffer
{
protected:
	int m_nWidth;
	int m_nHeight;
	Vector2 Size;

	Pixel* m_pBuffer;

public:
	Buffer(int width, int height) :
		m_nWidth(width),
		m_nHeight(height),
		Size(width, height),
		m_pBuffer(NULL)
	{
		m_pBuffer = new Pixel[m_nWidth * m_nHeight];
	};
	
	~Buffer()
	{
		delete m_pBuffer;
	}

	Vector2 getSize()
	{
		return Size;
	}

	int getWidth()
	{
		return m_nWidth;
	}

	int getHeight()
	{
		return m_nHeight;
	}

	Pixel getPixel(int nX, int nY)
	{
		return m_pBuffer[nX + m_nWidth * nY];
	}

	void setPixel(int nX, int nY, int nColor)
	{
		m_pBuffer[nX + m_nWidth * nY].m_chChar = 219;
		m_pBuffer[nX + m_nWidth * nY].m_nColor = nColor;
	}

	void set(string str, int nX, int nY, int nColor)
	{
		for (int nI = 0; nI < str.length(); nI++)
		{
			m_pBuffer[nX + nI + m_nWidth * nY].m_chChar = str[nI];
			m_pBuffer[nX + nI + m_nWidth * nY].m_nColor = nColor;
		}
	}

	void set(char ch, int nX, int nY, int nColor)
	{
		m_pBuffer[nX + m_nWidth * nY].m_chChar = ch;
		m_pBuffer[nX + m_nWidth * nY].m_nColor = nColor;
		
	}

	void set(Pixel pixel , int nX, int nY)
	{
		m_pBuffer[nX + m_nWidth * nY].set(pixel);
	}

	void set(Buffer* pBuffer, int nPosX, int nPosY)
	{
		for (int nY = 0; nY < pBuffer->getHeight(); nY++)
			for (int nX = 0; nX < pBuffer->getWidth(); nX++)
			{
				int nTotalY = nY + nPosY;
				int nTotalX = nX + nPosX;
				if ((nTotalY < m_nHeight && nTotalX < m_nWidth) &&			//	Protects from setting data outside the range of the frame
					(nTotalY >= 0 && nTotalX >= 0))
				{
					set(pBuffer->getPixel(nX, nY), nTotalX, nTotalY);
				}
			}
	}

	void clear(int nBGColor)
	{
		for (int nY = 0; nY < m_nHeight; ++nY)
			for (int nX = 0; nX < m_nWidth; ++nX)
			{
				m_pBuffer[nX + m_nWidth * nY].set(' ', nBGColor);
			}
	}
};