

class ConsoleOutputBuffer : public OutputBuffer
{
protected:
	Pixel* m_pBuffer;

public:
	ConsoleOutputBuffer(int width, int height, int pwidth, int pheight) :
		m_pBuffer(NULL),
		OutputBuffer(width, height, pwidth, pheight)
	{
		m_pBuffer = new Pixel[m_nWidth * m_nHeight];
	};

	~ConsoleOutputBuffer()
	{
		delete m_pBuffer;
	}

	void resize(int nWidth, int nHeight)
	{
		m_nWidth = nWidth;
		m_nHeight = nHeight;
		Size = { (float)nWidth, (float)nHeight };
		delete m_pBuffer;
		m_pBuffer = new Pixel[m_nWidth * m_nHeight];
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

	//	set string to buffer
	void set(string str, int nX, int nY, int nColor)
	{
		if (nX >= 0 && nY >= 0 && nY < m_nHeight)
			for (int nI = 0; nI < str.length(); nI++)
			{
				if (nX + nI < m_nWidth)
				{
					m_pBuffer[nX + nI + m_nWidth * nY].m_chChar = str[nI];
					m_pBuffer[nX + nI + m_nWidth * nY].m_nColor = nColor;
				}

			}
	}

	//	set single char to buffer
	void set(char ch, int nX, int nY, int nColor)
	{
		m_pBuffer[nX + m_nWidth * nY].m_chChar = ch;
		m_pBuffer[nX + m_nWidth * nY].m_nColor = nColor;
	}

	//	set pixel to buffer
	void set(Pixel pixel, int nX, int nY)
	{
		m_pBuffer[nX + m_nWidth * nY].set(pixel);
	}

	//	set another buffer into this buffer
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

	//	clear whole buffer to color
	void clear(int nBGColor)
	{
		for (int nY = 0; nY < m_nHeight; ++nY)
			for (int nX = 0; nX < m_nWidth; ++nX)
			{
				m_pBuffer[nX + m_nWidth * nY].set(' ', nBGColor);
			}
	}

};