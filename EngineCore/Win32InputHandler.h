class Win32InputHandler : public PlatformInputHandler
{
public:
	void processInput() 
	{
		MSG Message;
		BOOL MessageResult = GetMessage(&Message, 0, 0, 0);
		if (MessageResult > 0)
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	};
};
