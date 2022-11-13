class Win32Buffer : public OutputBuffer
{
private:

public:
	BITMAPINFO Info;
	void* pBuffer;
	int nWidth;
	int nHeight;
	int nBytesPerPixel = 4;
	HDC DeviceContext;
	RECT WindowSize;

	Win32Buffer(int width, int height, int pixWidth, int pixHeight) :
		OutputBuffer(width, height, pixWidth, pixHeight) {};

	void ResizeBuffer(int width, int height)
	{
		//	https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-createdibsection
		if (pBuffer)
		{
			VirtualFree(pBuffer, 0, MEM_RELEASE);
		}

		nWidth = width;
		nHeight = height;

		Info.bmiHeader.biSize = sizeof(Info.bmiHeader);
		Info.bmiHeader.biWidth = width;
		Info.bmiHeader.biHeight = -height;
		Info.bmiHeader.biPlanes = 1;
		Info.bmiHeader.biBitCount = 32;
		Info.bmiHeader.biCompression = BI_RGB;

		int nBitmapMemorySize = (width * height) * nBytesPerPixel;
		pBuffer = VirtualAlloc(0, nBitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
	}
};