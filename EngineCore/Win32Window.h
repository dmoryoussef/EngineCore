
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
			//	resizes buffer to match window size
			//	RECT rec;
			//	GetClientRect(Window, &rec);
			//	int nWidth = rec.right - rec.left;
			//	int nHeight = rec.bottom - rec.top;
			//	ResizeBuffer(nHeight, nWidth);
		}
		break;
		/*case WM_PAINT:
		{
			PAINTSTRUCT Paint;
			HDC DeviceContext = BeginPaint(Window, &Paint);
			int X = Paint.rcPaint.left;
			int Y = Paint.rcPaint.top;
			int Width = Paint.rcPaint.right - Paint.rcPaint.left;
			int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;

			RECT rec;
			GetClientRect(Window, &rec);
			UpdateWindow(DeviceContext, rec);
			EndPaint(Window, &Paint);
		}
		break;*/

		case WM_DESTROY:
		{
			OutputDebugString("WM_DESTROY \n");
		}
		break;
		/*case WM_CLOSE:
		{
			OutputDebugString("WM_CLOSE \n");
		}
		break;*/
		case WM_ACTIVATEAPP:
		{
			OutputDebugString("WM_ACTIVATEAPP \n");
		}
		break;
		default:
		{
			Result = DefWindowProc(Window, Message, WParam, LParam);
		}
		break;
	}

	return Result;
}

class Win32Window : public OutputWindow
{
private:
	HWND WindowHandle;
	BITMAPINFO Info;
	void* pBuffer;
	int nWidth;
	int nHeight;
	int nBytesPerPixel = 4;
	HDC DeviceContext;
	RECT WindowSize;


	void GetWindowSize()
	{
		GetClientRect(WindowHandle, &WindowSize);
	}

	void ResizeBuffer(int Height, int Width)
	{
		//	https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-createdibsection
		if (pBuffer)
		{
			VirtualFree(pBuffer, 0, MEM_RELEASE);
		}

		nWidth = Width;
		nHeight = Height;

		Info.bmiHeader.biSize = sizeof(Info.bmiHeader);
		Info.bmiHeader.biWidth = nWidth;
		Info.bmiHeader.biHeight = -nHeight;
		Info.bmiHeader.biPlanes = 1;
		Info.bmiHeader.biBitCount = 32;
		Info.bmiHeader.biCompression = BI_RGB;

		int nBitmapMemorySize = (nWidth * nHeight) * nBytesPerPixel;
		pBuffer = VirtualAlloc(0, nBitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
	}



public:
	Win32Window(int width, int height, int pwidth, int pheight) :
		OutputWindow(width, height, pwidth, pheight)
	{

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
			OutputDebugString("RegisterClass error");
		}
		//	https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa
		WindowHandle =
			CreateWindowEx(
				0,
				WindowClass.lpszClassName,
				"Win32 Window",
				WS_OVERLAPPEDWINDOW | WS_VISIBLE,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				0,
				0,
				hInstance,
				0);

		if (!WindowHandle)
		{
			OutputDebugString("Window not created");
		}
	}

	void outputToWindow()
	{
		//	https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-stretchdibits
		GetWindowSize();
		int nWindowHeight = WindowSize.bottom - WindowSize.top;
		int nWindowWidth = WindowSize.right - WindowSize.left;
		StretchDIBits(DeviceContext,
			0, 0, nWindowWidth, nWindowHeight,
			0, 0, nWidth, nHeight,
			pBuffer,
			&Info,
			DIB_RGB_COLORS, SRCCOPY);
	}
};