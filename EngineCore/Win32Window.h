
struct Win32Buffer
{
	BITMAPINFO Info;
	void* pBuffer;
	int nWidth;
	int nHeight;
	int nBytesPerPixel = 4;
	HDC DeviceContext;
	RECT WindowSize;
} win32Buffer;

void ResizeBuffer(int Height, int Width)
{
	//	https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-createdibsection
	if (win32Buffer.pBuffer)
	{
		VirtualFree(win32Buffer.pBuffer, 0, MEM_RELEASE);
	}

	win32Buffer.nWidth = Width;
	win32Buffer.nHeight = Height;

	win32Buffer.Info.bmiHeader.biSize = sizeof(win32Buffer.Info.bmiHeader);
	win32Buffer.Info.bmiHeader.biWidth = win32Buffer.nWidth;
	win32Buffer.Info.bmiHeader.biHeight = -win32Buffer.nHeight;
	win32Buffer.Info.bmiHeader.biPlanes = 1;
	win32Buffer.Info.bmiHeader.biBitCount = 32;
	win32Buffer.Info.bmiHeader.biCompression = BI_RGB;

	int nBitmapMemorySize = (win32Buffer.nWidth * win32Buffer.nHeight) * win32Buffer.nBytesPerPixel;
	win32Buffer.pBuffer = VirtualAlloc(0, nBitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
}

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
			
			//	resizes output buffer to match window size
			RECT rec;
			GetClientRect(Window, &rec);
			int nWidth = rec.right - rec.left;
			int nHeight = rec.bottom - rec.top;
			//	ResizeBuffer(nHeight, nWidth);
		}
		break;

		case WM_PAINT:
		{
			//PAINTSTRUCT Paint;
			//HDC DeviceContext = BeginPaint(Window, &Paint);
			//int X = Paint.rcPaint.left;
			//int Y = Paint.rcPaint.top;
			//int Width = Paint.rcPaint.right - Paint.rcPaint.left;
			//int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;

			//RECT rec;
			//GetClientRect(Window, &rec);
			//UpdateWindow(DeviceContext, rec);
			//EndPaint(Window, &Paint);
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

public:
	Win32Window(int width, int height, int pwidth, int pheight) :
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
	{
		for (int nY = 0; nY < pBuffer->getHeight(); ++nY)
			for (int nX = 0; nX < pBuffer->getWidth(); ++nX)
				if ((nY < m_nScreenHeight && nX < m_nScreenWidth) &&
					(nY >= 0 && nX >= 0))
				{
					Pixel current = pBuffer->getPixel(nX, nY);
					//	convert pixel to window screen buffer here
				}
	}

	void outputToWindow()
	{
		//	https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-stretchdibits
		//
		//int nWindowHeight = WindowSize.bottom - WindowSize.top;
		//int nWindowWidth = WindowSize.right - WindowSize.left;
		/*StretchDIBits(DeviceContext,
			0, 0, nWindowWidth, nWindowHeight,
			0, 0, nWidth, nHeight,
			pBuffer,
			&Info,
			DIB_RGB_COLORS, SRCCOPY);*/
	}
};