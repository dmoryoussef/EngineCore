class OutputWindow
{
protected:
	int m_nScreenHeight;
	int m_nScreenWidth;
	int m_nPixelWidth;
	int m_nPixelHeight;

public:
	OutputWindow(int width, int height, int pixelWidth, int pixelHeight) :
		m_nScreenWidth(width),
		m_nScreenHeight(height),
		m_nPixelWidth(pixelWidth),
		m_nPixelHeight(pixelHeight)
	{};

	int getHeight()
	{
		return m_nScreenHeight;
	}

	int getWidth()
	{
		return m_nScreenWidth;
	}

	virtual void init() = 0;

	virtual void outputToWindow() = 0;

	virtual void renderToBuffer(OutputBuffer *pBuffer) = 0;

};