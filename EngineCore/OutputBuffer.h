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

class OutputBuffer
{
protected:
	int m_nWidth;
	int m_nHeight;
	Vector2 Size;

	int m_nPixelWidth;
	int m_nPixelHeight;

public:
	OutputBuffer(int width, int height,int pixelWidth, int pixelHeight) :
		m_nWidth(width),
		m_nHeight(height),
		m_nPixelWidth(pixelWidth),
		m_nPixelHeight(pixelHeight),
		Size(width, height)	{};
	
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

	virtual void resize(int nWidth, int nHeight) {}

	virtual Pixel getPixel(int nX, int nY) { return Pixel(); }

	virtual void setPixel(int nX, int nY, int nColor) {}

	//	set string to buffer
	virtual void set(string str, int nX, int nY, int nColor) {}

	//	set single char to buffer
	virtual void set(char ch, int nX, int nY, int nColor) {}

	//	set pixel to buffer
	virtual void set(Pixel pixel , int nX, int nY) {}

	//	set another buffer into this buffer
	virtual void set(OutputBuffer* pBuffer, int nPosX, int nPosY) {}

	//	clear whole buffer to color
	virtual void clear(int nBGColor) {}
};