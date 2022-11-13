class OutputWindow
{
protected:
	int m_nScreenHeight;
	int m_nScreenWidth;

public:
	OutputWindow(int width, int height) :
		m_nScreenWidth(width),
		m_nScreenHeight(height) {};

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