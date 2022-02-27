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

	Pixel* m_pBuffer;

public:
	Buffer(int width, int height) :
		m_nWidth(width),
		m_nHeight(height),
		m_pBuffer(NULL)
	{
		m_pBuffer = new Pixel[m_nWidth * m_nHeight];
	};
	
	~Buffer()
	{
		delete m_pBuffer;
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
		m_pBuffer[nX + m_nWidth * nY].m_chChar = 'A';
		m_pBuffer[nX + m_nWidth * nY].m_nColor = nColor;
	}

};