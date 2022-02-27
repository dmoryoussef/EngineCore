

class ConsoleBuffer : public Buffer
{
private:

public:
	ConsoleBuffer(int width, int height) :
		Buffer(width, height) 
	{	
		m_pBuffer = new Pixel[m_nWidth * m_nHeight];
	}
};