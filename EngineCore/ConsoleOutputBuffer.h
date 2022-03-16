

class ConsoleOutputBuffer : public OutputBuffer
{
private:

public:
	ConsoleOutputBuffer(int width, int height) :
		OutputBuffer(width, height) 
	{	
		m_pBuffer = new Pixel[m_nWidth * m_nHeight];
	}
};