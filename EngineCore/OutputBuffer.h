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

	Pixel* m_pBuffer;

public:
	OutputBuffer(int width, int height) :
		m_nWidth(width),
		m_nHeight(height),
		Size(width, height),
		m_pBuffer(NULL)
	{
		m_pBuffer = new Pixel[m_nWidth * m_nHeight];
	};
	
	~OutputBuffer()
	{
		delete m_pBuffer;
	}

	void resize(int nWidth, int nHeight)
	{
		m_nWidth = nWidth;
		m_nHeight = nHeight;
		delete m_pBuffer;
		m_pBuffer = new Pixel[m_nWidth * m_nHeight];
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

	void set(OutputBuffer* pBuffer, int nPosX, int nPosY)
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