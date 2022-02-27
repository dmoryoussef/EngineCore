#include <Windows.h>

class ConsoleScreenBuffer : public ScreenBuffer
{
private:
	HANDLE m_hOutputHandle;
	CHAR_INFO* m_pBuffer;

public:	
	ConsoleScreenBuffer(int width, int height, int pwidth, int pheight) :
		m_pBuffer(NULL),
		ScreenBuffer(width, height, pwidth, pheight) 
	{
		m_hOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	};


	void resizeBuffer(int nWidth, int nHeight)
	{
		m_nScreenHeight = nHeight;
		m_nScreenWidth = nWidth;
		delete m_pBuffer;
		m_pBuffer = new CHAR_INFO[nHeight * nWidth];
	}

	void renderToBuffer()
	{
		//for (int nY = 0; nY < pFrame->getSize().Y; ++nY)
		//	for (int nX = 0; nX < pFrame->getSize().X; ++nX)
		//		if ((nY < m_nHeight && nX < m_nWidth) &&			//	Protects from setting data outside the range of the frame
		//			(nY >= 0 && nX >= 0))
		//		{
		//			m_pBuffer[nX + m_nWidth * nY].Char.AsciiChar = pFrame->getAPixel(nX, nY).m_chChar;
		//			m_pBuffer[nX + m_nWidth * nY].Attributes = pFrame->getAPixel(nX, nY).m_nColor;
		//		}

		m_pBuffer[2 + m_nScreenWidth * 2].Char.AsciiChar = 'A';
		m_pBuffer[2 + m_nScreenWidth * 2].Attributes = 15;
	}

	void init()
	{
		//	set size of buffer position and size
		//	https://docs.microsoft.com/en-us/windows/console/setconsolewindowinfo
		SMALL_RECT m_rectWindow = { 0, 0, 1, 1 };
		SetConsoleWindowInfo(m_hOutputHandle, TRUE, &m_rectWindow);

		// set screen buffer size
		//	https://docs.microsoft.com/en-us/windows/console/setconsolescreenbuffersize
		COORD coord = { (short)m_nScreenWidth, (short)m_nScreenHeight };
		if (!SetConsoleScreenBufferSize(m_hOutputHandle, coord))
		{
			OutputDebugStringA("SetConsoleScreenBufferSize \n");
		}

		// Assign screen buffer to the console
		//	https://docs.microsoft.com/en-us/windows/console/setconsoleactivescreenbuffer
		if (!SetConsoleActiveScreenBuffer(m_hOutputHandle))
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
		if (!GetConsoleScreenBufferInfo(m_hOutputHandle, &csbi))
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
		if (!SetConsoleWindowInfo(m_hOutputHandle, TRUE, &m_rectWindow))
		{
			OutputDebugStringA("SetConsoleWindowInfo \n");
		}

		// Set flags to allow mouse input		
		//	https://docs.microsoft.com/en-us/windows/console/setconsolemode
		if (!SetConsoleMode(m_hOutputHandle, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT))
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

		WriteConsoleOutput(m_hOutputHandle, m_pBuffer, charBufSize, characterPos, &writeArea);
	}

};
