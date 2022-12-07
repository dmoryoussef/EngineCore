
class Win32Buffer : public OutputBuffer
{
private:
	int nBytesPerPixel = 4;
	RECT WindowSize;

public:
	Win32Buffer(int width, int height, int pwidth, int pheight) :
		OutputBuffer(width, height, pwidth, pheight)
	{
		DeviceContext = CreateCompatibleDC(0);
	}
	void* pBuffer;
	BITMAPINFO Info;
	HDC DeviceContext;

	void renderToBuffer(OutputBuffer* pBuffer)
	{
		for (int nY = 0; nY < pBuffer->getHeight(); ++nY)
			for (int nX = 0; nX < pBuffer->getWidth(); ++nX)
				if ((nY < m_nHeight && nX < m_nWidth) &&
					(nY >= 0 && nX >= 0))
				{
					Pixel current = pBuffer->getPixel(nX, nY);
					//	convert pixel to window screen buffer here
				}
	}

	void resize(int width, int height)
	{
		//	https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-createdibsection
		if (pBuffer)
		{
			//	delete pBuffer;
			VirtualFree(pBuffer, 0, MEM_RELEASE);
		}

		m_nWidth = width;
		m_nHeight = height;

		Info.bmiHeader.biSize = sizeof(Info.bmiHeader);
		Info.bmiHeader.biWidth = m_nWidth;
		Info.bmiHeader.biHeight = -m_nHeight;
		Info.bmiHeader.biPlanes = 1;
		Info.bmiHeader.biBitCount = 32;
		Info.bmiHeader.biCompression = BI_RGB;

		int nBitmapMemorySize = (m_nWidth * m_nHeight) * nBytesPerPixel;
		pBuffer = VirtualAlloc(0, nBitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);

		int pitch = m_nWidth * nBytesPerPixel;
		uint8_t *row = (uint8_t*)pBuffer;
		for (int y = 0; y < m_nHeight; ++y)
		{
			uint32_t* pixel = (uint32_t*)row;
			for (int x = 0; x < m_nWidth; ++x)
			{
				//		   RR GG BB xx
				//	Pixel: 00 00 00 00 
	/*			*pixel = 255;
				++pixel;

				*pixel = 0;
				++pixel;

				*pixel = 0;
				++pixel;

				*pixel = 0;
				++pixel;*/

				uint8_t Blue = 0;
				uint8_t Green = 255;
				uint8_t Red = 0;

				*pixel++ = (Red << 16 | Green << 8 | Blue);

			}

			row += pitch;
		}


	}

};

Win32Buffer Win32OutputBuffer(720, 480, 4, 4);



LRESULT CALLBACK
MainWindowCallback(HWND Window,
				   UINT Message,
				   WPARAM WParam,
				   LPARAM LParam)
{
	LRESULT Result = 0;
	switch (Message)
	{
		case WM_SIZE:
		{
			cout << ("WM_SIZE \n");

			//	resize output buffer to match window size
			RECT rec;
			GetClientRect(Window, &rec);
			int nWidth = rec.right - rec.left;
			int nHeight = rec.bottom - rec.top;
			Win32OutputBuffer.resize(nWidth, nHeight);
		}
		break;

		case WM_PAINT:
		{
			PAINTSTRUCT Paint;
			BeginPaint(Window, &Paint);
			EndPaint(Window, &Paint);
			cout << ("WM_PAINT \n");
		}
		break;

		case WM_DESTROY:
		{
			cout << ("WM_DESTROY \n");
		} break;

		case WM_CLOSE:
		{
			cout << ("WM_CLOSE \n");
		} break;

		case WM_ACTIVATEAPP:
		{
			cout << ("WM_ACTIVATEAPP \n");
		} break;

		default:
		{
			Result = DefWindowProc(Window, Message, WParam, LParam);
		} break;

	}
	return Result;
}



class Win32Window : public OutputWindow
{
private:
	HWND WindowHandle;
	//	Win32Buffer BackBuffer;

public:
	Win32Window(int width, int height, int pwidth, int pheight) :
		//	BackBuffer(width, height, pwidth, pheight),
		OutputWindow(width, height, pwidth, pheight) {};

	void init()
	{
		//	https://docs.microsoft.com/en-us/windows/win32/learnwin32/creating-a-window
		HINSTANCE hInstance = GetModuleHandle(0);
		WNDCLASS WindowClass = {};

		WindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		WindowClass.lpfnWndProc = MainWindowCallback;
		WindowClass.hInstance = hInstance;
		WindowClass.lpszClassName = "Win32 Window Class";

		if (!RegisterClass(&WindowClass))
		{
			OutputDebugStringA("RegisterClass error");
		}
		//	https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa
		WindowHandle =
			CreateWindowEx(
				0,
				WindowClass.lpszClassName,
				"Win32 Window",
				WS_OVERLAPPEDWINDOW | WS_VISIBLE,
				500,	// screen position x
				500,	// screen position y
				m_nScreenWidth,
				m_nScreenHeight,
				0,
				0,
				hInstance,
				0);

		if (!WindowHandle)
		{
			OutputDebugStringA("Window not created");
		}
	}

	void renderToBuffer(OutputBuffer* pBuffer)
	{}

	void outputToWindow()
	{
		//	https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-stretchdibits
		RECT rec;
		GetClientRect(WindowHandle, &rec);
		int nWindowHeight = rec.bottom - rec.top;
		int nWindowWidth = rec.right - rec.left;

		Win32Buffer BackBuffer = Win32OutputBuffer;

		HDC dc = GetDC(WindowHandle);
		StretchDIBits(dc,
			0, 0, nWindowWidth, nWindowHeight,
			0, 0, BackBuffer.getWidth(), BackBuffer.getHeight(),
			BackBuffer.pBuffer,
			&BackBuffer.Info,
			DIB_RGB_COLORS, SRCCOPY);

		ReleaseDC(WindowHandle, dc);
	}
};