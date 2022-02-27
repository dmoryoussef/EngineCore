class ScreenBuffer
{
protected:
	int m_nScreenHeight;
	int m_nScreenWidth;

	int m_nPixelHeight;
	int m_nPixelWidth;

public:
	ScreenBuffer(int width, int height, int pwidth, int pheight) :
		m_nScreenWidth(width),
		m_nScreenHeight(height),
		m_nPixelHeight(pheight),
		m_nPixelWidth(pwidth) {};

	virtual void init() = 0;

};