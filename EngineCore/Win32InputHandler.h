class Win32InputHandler : public PlatformInputHandler
{
public:

	void processInput() 
	{
		MSG Message;
		while (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
		{
			if (Message.message == WM_QUIT)
			{
				//	close window
			}

			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	};
};
