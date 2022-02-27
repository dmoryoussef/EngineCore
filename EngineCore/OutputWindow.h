class OutputWindow
{
protected:
	int m_nScreenHeight;
	int m_nScreenWidth;

	int m_nPixelHeight;
	int m_nPixelWidth;

public:
	OutputWindow(int width, int height, int pwidth, int pheight) :
		m_nScreenWidth(width),
		m_nScreenHeight(height),
		m_nPixelHeight(pheight),
		m_nPixelWidth(pwidth) {};

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

	virtual void renderToBuffer(Buffer *pBuffer) = 0;

};