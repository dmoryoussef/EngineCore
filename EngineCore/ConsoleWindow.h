#include <Windows.h>

class ConsoleWindow : public OutputWindow
{
private:
	HANDLE WindowHandle;
	CHAR_INFO *m_pBuffer;

public:	
	ConsoleWindow(int width, int height, int pwidth, int pheight) :
		m_pBuffer(NULL),
		OutputWindow(width, height, pwidth, pheight) 
	{
		WindowHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	};


	void resizeBuffer(int nWidth, int nHeight)
	{
		m_nScreenHeight = nHeight;
		m_nScreenWidth = nWidth;
		delete m_pBuffer;
		m_pBuffer = new CHAR_INFO[m_nScreenHeight * m_nScreenWidth];
	}

	void renderToBuffer(Buffer *pBuffer)
	{
		for (int nY = 0; nY < pBuffer->getHeight(); ++nY)
			for (int nX = 0; nX < pBuffer->getWidth(); ++nX)
				if ((nY < m_nScreenHeight && nX < m_nScreenWidth) &&			
					(nY >= 0 && nX >= 0))
				{
					Pixel current = pBuffer->getPixel(nX, nY);
					m_pBuffer[nX + m_nScreenWidth * nY].Char.AsciiChar = current.m_chChar;
					m_pBuffer[nX + m_nScreenWidth * nY].Attributes = current.m_nColor;
				}
	}

	void init()
	{
		//	set size of buffer position and size
		//	https://docs.microsoft.com/en-us/windows/console/setconsolewindowinfo
		SMALL_RECT m_rectWindow = { 0, 0, 1, 1 };
		SetConsoleWindowInfo(WindowHandle, TRUE, &m_rectWindow);

		// set screen buffer size
		//	https://docs.microsoft.com/en-us/windows/console/setconsolescreenbuffersize
		COORD coord = { (short)m_nScreenWidth, (short)m_nScreenHeight };
		if (!SetConsoleScreenBufferSize(WindowHandle, coord))
		{
			OutputDebugStringA("SetConsoleScreenBufferSize \n");
		}

		// Assign screen buffer to the console
		//	https://docs.microsoft.com/en-us/windows/console/setconsoleactivescreenbuffer
		if (!SetConsoleActiveScreenBuffer(WindowHandle))
		{
			OutputDebugStringA("SetConsoleActiveScreenBuffer \n");
		}

		// Set the font size now that the screen buffer has been assigned to the console
		//	https://docs.microsoft.com/en-us/windows/console/console-font-infoex
		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = m_nPixelWidth;
		cfi.dwFontSize.Y = m_nPixelHeight;
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;

		//	https://docs.microsoft.com/en-us/windows/console/setcurrentconsolefontex
		wcscpy_s(cfi.FaceName, L"Consolas");
		//if (!SetCurrentConsoleFontEx(m_hOutputHandle, false, &cfi))
		{
			OutputDebugStringA("SetCurrentConsoleFontEx \n");
		}

		// Get screen buffer info and check the maximum allowed window size. Return
		// error if exceeded, so user knows their dimensions/fontsize are too large
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		//	https://docs.microsoft.com/en-us/windows/console/getconsolescreenbufferinfo
		if (!GetConsoleScreenBufferInfo(WindowHandle, &csbi))
		{
			OutputDebugStringA("GetConsoleScreenBufferInfo \n");
		}

		if (m_nScreenHeight > csbi.dwMaximumWindowSize.Y)
		{
			OutputDebugStringA("Screen Height / Font Height Too Big \n");
		}

		if (m_nScreenWidth > csbi.dwMaximumWindowSize.X)
		{
			OutputDebugStringA("Screen Width / Font Width Too Big \n");
		}

		// Set Physical Console Window Size
		m_rectWindow = { 0, 0, (short)m_nScreenWidth - 1, (short)m_nScreenHeight - 1 };
		if (!SetConsoleWindowInfo(WindowHandle, TRUE, &m_rectWindow))
		{
			OutputDebugStringA("SetConsoleWindowInfo \n");
		}

		// Set flags to allow mouse input		
		//	https://docs.microsoft.com/en-us/windows/console/setconsolemode
		if (!SetConsoleMode(WindowHandle, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT))
		{
			OutputDebugStringA("SetConsoleMode \n");
		}

		// Allocate memory for screen buffer
		//	m_pBuffer = new CHAR_INFO[nWindowWidth * nWindowHeight];
		resizeBuffer(m_nScreenWidth, m_nScreenHeight);
		//	memset(m_pBuffer, 0, sizeof(CHAR_INFO) * nWindowWidth * nWindowHeight);

		//	https://docs.microsoft.com/en-us/windows/console/setconsolectrlhandler
		//	SetConsoleCtrlHandler((PHANDLER_ROUTINE)CloseHandler, TRUE);

	}

	void outputToWindow()
	{
		COORD charBufSize = { m_nScreenWidth, m_nScreenHeight };
		COORD characterPos = { 0, 0 };
		SMALL_RECT writeArea = { 0, 0, m_nScreenWidth - 1, m_nScreenHeight - 1 };

		WriteConsoleOutput(WindowHandle, m_pBuffer, charBufSize, characterPos, &writeArea);
	}

};
