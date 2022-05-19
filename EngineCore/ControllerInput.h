#include <xinput.h>
#include <Windows.h>

#define X_INPUT_SET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE *pState)
typedef X_INPUT_SET_STATE(x_input_set_state);
X_INPUT_SET_STATE(XInputSetStateStub) { return(0); }
x_input_set_state* _XInputSetState = XInputSetStateStub;
#define XInputSetState _XInputSetState

#define X_INPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE *pState)
typedef X_INPUT_GET_STATE(x_input_get_state);
X_INPUT_GET_STATE(XInputGetStateStub) { return(0); }
x_input_get_state* _XInputGetState = XInputGetStateStub;
#define XInputGetState _XInputGetState




class ControllerInput :
	public EventListener
{
private:
	bool controllerConnected[XUSER_MAX_COUNT];

public:
	ControllerInput() {}

	void update()
	{
		DWORD dwResult;
		for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
		{
			XINPUT_STATE state;
			ZeroMemory(&state, sizeof(XINPUT_STATE));

			// Simply get the state of the controller from XInput.
			dwResult = _XInputGetState(i, &state);

			if (dwResult == ERROR_SUCCESS)
			{
				if (controllerConnected[i] != true)
				{
					OutputDebugString("Controller is connected\n");
					controllerConnected[i] = true;
				}
			}
			else
			{
				if (controllerConnected[i] == true)
				{
					OutputDebugString("Controller is not connected\n");
					controllerConnected[i] = false;
				}
			}
		}
	}

	void loadXInput()
	{
		HMODULE XInputLibrary = LoadLibrary("xinput1_3.dll");
		if (XInputLibrary)
		{
			XInputGetState = (x_input_get_state*)GetProcAddress(XInputLibrary, "XInputGetState");
			XInputSetState = (x_input_set_state*)GetProcAddress(XInputLibrary, "XInputSetState");
		}
	}

	void handleControllerInput()
	{
		DWORD dwResult;
		for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
		{
			XINPUT_STATE state;
			ZeroMemory(&state, sizeof(XINPUT_STATE));
			update();

			for (DWORD ControllerIndex = 0;
				ControllerIndex < XUSER_MAX_COUNT;
				++ControllerIndex)
			{
				XINPUT_STATE ControllerState;
				if (XInputGetState(ControllerIndex, &ControllerState) == ERROR_SUCCESS)
				{
					XINPUT_GAMEPAD* pGamePad = &ControllerState.Gamepad;

				
					GamePad Controller;
					Controller.nIndex = ControllerIndex;
					Controller.Up = (pGamePad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
					Controller.Down = (pGamePad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
					Controller.Left = (pGamePad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
					Controller.Right = (pGamePad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
					Controller.Start = (pGamePad->wButtons & XINPUT_GAMEPAD_START);
					Controller.Back = (pGamePad->wButtons & XINPUT_GAMEPAD_BACK);
					Controller.LeftShoulder = (pGamePad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
					Controller.RightShoulder = (pGamePad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
					Controller.AButton = (pGamePad->wButtons & XINPUT_GAMEPAD_A);
					Controller.BButton = (pGamePad->wButtons & XINPUT_GAMEPAD_B);
					Controller.XButton = (pGamePad->wButtons & XINPUT_GAMEPAD_X);
					Controller.YButton = (pGamePad->wButtons & XINPUT_GAMEPAD_Y);

					Controller.LeftTrigger = (pGamePad->bLeftTrigger & XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
					Controller.RightTrigger = (pGamePad->bRightTrigger & XINPUT_GAMEPAD_TRIGGER_THRESHOLD);

					//	stick range is between -32768 and 32767
					//	normalize first
					Controller.LeftStickX = (float)pGamePad->sThumbLX / 32768.0;
					Controller.LeftStickY = (float)pGamePad->sThumbLY / 32768.0;
					Controller.RightStickX = (float)pGamePad->sThumbRX / 32768.0;
					Controller.RightStickY = (float)pGamePad->sThumbRY / 32768.0;

					// clear deadzone range
					Controller.LeftStickX = Controller.clearDeadzone(Controller.LeftStickX);
					Controller.LeftStickY = Controller.clearDeadzone(Controller.LeftStickY);
					Controller.RightStickX = Controller.clearDeadzone(Controller.RightStickX);
					Controller.RightStickY = Controller.clearDeadzone(Controller.RightStickY);
					
					//	only send event if SOMETHING is pressed on controller
					//	right now only stick movement is tested:
					if (Controller.LeftStickX != 0.0 ||
						Controller.LeftStickY != 0.0 ||
						Controller.RightStickX != 0.0 ||
						Controller.RightStickY != 0.0 ||
						Controller.LeftTrigger != 0.0 ||
						Controller.RightTrigger != 0.0 ||
						Controller.XButton)

						addEvent(new GamePadEvent(Controller));
				}
			}

		}
	}
};

