#include "ControllerInput.h"

const LONG ControllerInput::zoneMax = +1000;
const LONG ControllerInput::zoneMin = -1000;
IDirectInput8* ControllerInput::dinput = nullptr;
ControllerInput::vector<IDirectInputDevice8*> ControllerInput::devGamepad = {};
ControllerInput::vector<DIJOYSTATE> ControllerInput::gamepad = {};
ControllerInput::vector<DIJOYSTATE> ControllerInput::oldgamepad = {};
size_t ControllerInput::pressCount[64] = {};
size_t ControllerInput::releaseCount[64] = {};

ControllerInput* ControllerInput::GetInstance()
{
	static ControllerInput instance;
	return &instance;
}

void ControllerInput::Init()
{
	HRESULT result;

	result = DirectInput8Create(
		WindowsAPI::GetInstance(), DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&dinput, nullptr);
	result=CreateGamepadDevice();
}

void ControllerInput::Update()
{
	HRESULT result;
	if (devGamepad.size() == 0)
	{
		result = CreateGamepadDevice();
		if (devGamepad.size() == 0)
		{
			return;
		}
	}

	for (size_t i = 0; i < devGamepad.size(); i++)
	{
		result = devGamepad[i]->Acquire();
		if (result == DIERR_INPUTLOST)
		{
			//デバイスがロストしていたら無視する
			continue;
		}

		devGamepad[i]->Poll();

		if (gamepad.size() < devGamepad.size())
		{
			do
			{
				//数合わせ
				gamepad.push_back({});
				oldgamepad.push_back({});
			} while (gamepad.size() != devGamepad.size());
		}

		oldgamepad[i] = gamepad[i];

		ZeroMemory(&gamepad[i], sizeof(DIJOYSTATE));
		devGamepad[i]->GetDeviceState(sizeof(DIJOYSTATE), &gamepad[i]);
	}

	static const size_t buttonCount = sizeof(gamepad[0].rgbButtons) / sizeof(gamepad[0].rgbButtons[0]);
	bool now = false;

	for (size_t i = 0; i < 64; i++)
	{
		if (i < buttonCount)
		{
			if (gamepad[0].rgbButtons[i])
			{
				if (releaseCount[i] != 0)
				{
					releaseCount[i] = 0;
				}
				pressCount[i]++;
			}
			else
			{
				if (pressCount[i] != 0)
				{
					pressCount[i] = 0;
				}
				releaseCount[i]++;
			}
		}
		else if (i == buttonCount + 4)
		{
			if (i == XBOX_INPUT_LEFT)
			{
				now = gamepad[0].rgdwPOV[0] == 4500 * 5 ||
					gamepad[0].rgdwPOV[0] == 4500 * 6 ||
					gamepad[0].rgdwPOV[0] == 4500 * 7;
			}
			else if (i == XBOX_INPUT_RIGHT)
			{
				now = gamepad[0].rgdwPOV[0] == 4500 * 1 ||
					gamepad[0].rgdwPOV[0] == 4500 * 2 ||
					gamepad[0].rgdwPOV[0] == 4500 * 3;
			}
			else if (i == XBOX_INPUT_UP)
			{
				now = gamepad[0].rgdwPOV[0] == 4500 * 7 ||
					gamepad[0].rgdwPOV[0] == 4500 * 0 ||
					gamepad[0].rgdwPOV[0] == 4500 * 1;
			}
			else if (i == XBOX_INPUT_DOWN)
			{
				now = gamepad[0].rgdwPOV[0] == 4500 * 3 ||
					gamepad[0].rgdwPOV[0] == 4500 * 4 ||
					gamepad[0].rgdwPOV[0] == 4500 * 5;
			}

			if (now)
			{
				if (releaseCount[i] != 0)
				{
					releaseCount[i] = 0;
				}
				pressCount[i]++;
			}
			else
			{
				if (pressCount[i] != 0)
				{
					pressCount[i] = 0;
				}
				releaseCount[i]++;
			}
		}
		else
		{
			if (pressCount[i] != 0)
			{
				pressCount[i] = 0;
			}
			releaseCount[i]++;
		}
	}
}

DIJOYSTATE ControllerInput::GetGamepadState(const size_t& padNo)
{
	if (padNo >= gamepad.size())
	{
		return DIJOYSTATE();
	}
	return gamepad[padNo];
}

bool ControllerInput::IsPadButton(const size_t& button, const size_t& padNo)
{
	if (padNo >= gamepad.size())
	{
		return false;
	}
	if (button >= sizeof(pressCount) / sizeof(pressCount[0]))
	{
		return false;
	}
	return pressCount[button] != 0;
}

size_t ControllerInput::GetPadButtonPress(const size_t& button, const size_t& padNo)
{
	if (padNo >= gamepad.size())
	{
		return 0;
	}
	if (button >= sizeof(pressCount) / sizeof(pressCount[0]))
	{
		return 0;
	}
	return pressCount[button];
}

size_t ControllerInput::GetPadButtonRelease(const size_t& button, const size_t& padNo)
{
	if (padNo >= gamepad.size())
	{
		return 0;
	}
	if (button >= sizeof(releaseCount) / sizeof(releaseCount[0]))
	{
		return 0;
	}
	return releaseCount[button];
}

bool ControllerInput::IsPadButtonTrigger(const size_t& button, const size_t& padNo)
{
	if (padNo >= gamepad.size())
	{
		return false;
	}
	if (button >= sizeof(DIJOYSTATE::rgbButtons) / sizeof(DIJOYSTATE::rgbButtons[0]) + 4)
	{
		return false;
	}

	if (button < sizeof(DIJOYSTATE::rgbButtons) / sizeof(DIJOYSTATE::rgbButtons[0]))
	{
		return gamepad[padNo].rgbButtons[button] && !oldgamepad[padNo].rgbButtons[button];
	}
	else if (button == XBOX_INPUT_LEFT)
	{
		bool now = gamepad[padNo].rgdwPOV[0] == 4500 * 5 ||
			gamepad[padNo].rgdwPOV[0] == 4500 * 6 ||
			gamepad[padNo].rgdwPOV[0] == 4500 * 7;

		bool old= oldgamepad[padNo].rgdwPOV[0] == 4500 * 5 ||
			oldgamepad[padNo].rgdwPOV[0] == 4500 * 6 ||
			oldgamepad[padNo].rgdwPOV[0] == 4500 * 7;

		return now && !old;
	}
	else if (button == XBOX_INPUT_RIGHT)
	{
		bool now = gamepad[padNo].rgdwPOV[0] == 4500 * 1 ||
			gamepad[padNo].rgdwPOV[0] == 4500 * 2 ||
			gamepad[padNo].rgdwPOV[0] == 4500 * 3;

		bool old = oldgamepad[padNo].rgdwPOV[0] == 4500 * 1 ||
			oldgamepad[padNo].rgdwPOV[0] == 4500 * 2 ||
			oldgamepad[padNo].rgdwPOV[0] == 4500 * 3;

		return now && !old;
	}
	else if (button == XBOX_INPUT_UP)
	{
		bool now = gamepad[padNo].rgdwPOV[0] == 4500 * 7 ||
			gamepad[padNo].rgdwPOV[0] == 4500 * 0 ||
			gamepad[padNo].rgdwPOV[0] == 4500 * 1;

		bool old = oldgamepad[padNo].rgdwPOV[0] == 4500 * 7 ||
			oldgamepad[padNo].rgdwPOV[0] == 4500 * 0 ||
			oldgamepad[padNo].rgdwPOV[0] == 4500 * 1;

		return now && !old;
	}
	else if (button == XBOX_INPUT_DOWN)
	{
		bool now = gamepad[padNo].rgdwPOV[0] == 4500 * 3 ||
			gamepad[padNo].rgdwPOV[0] == 4500 * 4 ||
			gamepad[padNo].rgdwPOV[0] == 4500 * 5;

		bool old = oldgamepad[padNo].rgdwPOV[0] == 4500 * 3 ||
			oldgamepad[padNo].rgdwPOV[0] == 4500 * 4 ||
			oldgamepad[padNo].rgdwPOV[0] == 4500 * 5;

		return now && !old;
	}

	return false;
}

bool ControllerInput::IsPadButtonRelease(const size_t& button, const size_t& padNo)
{
	if (padNo >= gamepad.size())
	{
		return false;
	}
	if (button >= sizeof(DIJOYSTATE::rgbButtons) / sizeof(DIJOYSTATE::rgbButtons[0]) + 4)
	{
		return false;
	}

	if (button < sizeof(DIJOYSTATE::rgbButtons) / sizeof(DIJOYSTATE::rgbButtons[0]))
	{
		return !gamepad[padNo].rgbButtons[button] && oldgamepad[padNo].rgbButtons[button];
	}
	else if (button == XBOX_INPUT_LEFT)
	{
		bool now = gamepad[padNo].rgdwPOV[0] == 4500 * 5 ||
			gamepad[padNo].rgdwPOV[0] == 4500 * 6 ||
			gamepad[padNo].rgdwPOV[0] == 4500 * 7;

		bool old = oldgamepad[padNo].rgdwPOV[0] == 4500 * 5 ||
			oldgamepad[padNo].rgdwPOV[0] == 4500 * 6 ||
			oldgamepad[padNo].rgdwPOV[0] == 4500 * 7;

		return !now && old;
	}
	else if (button == XBOX_INPUT_RIGHT)
	{
		bool now = gamepad[padNo].rgdwPOV[0] == 4500 * 1 ||
			gamepad[padNo].rgdwPOV[0] == 4500 * 2 ||
			gamepad[padNo].rgdwPOV[0] == 4500 * 3;

		bool old = oldgamepad[padNo].rgdwPOV[0] == 4500 * 1 ||
			oldgamepad[padNo].rgdwPOV[0] == 4500 * 2 ||
			oldgamepad[padNo].rgdwPOV[0] == 4500 * 3;

		return !now && old;
	}
	else if (button == XBOX_INPUT_UP)
	{
		bool now = gamepad[padNo].rgdwPOV[0] == 4500 * 7 ||
			gamepad[padNo].rgdwPOV[0] == 4500 * 0 ||
			gamepad[padNo].rgdwPOV[0] == 4500 * 1;

		bool old = oldgamepad[padNo].rgdwPOV[0] == 4500 * 7 ||
			oldgamepad[padNo].rgdwPOV[0] == 4500 * 0 ||
			oldgamepad[padNo].rgdwPOV[0] == 4500 * 1;

		return !now && old;
	}
	else if (button == XBOX_INPUT_DOWN)
	{
		bool now = gamepad[padNo].rgdwPOV[0] == 4500 * 3 ||
			gamepad[padNo].rgdwPOV[0] == 4500 * 4 ||
			gamepad[padNo].rgdwPOV[0] == 4500 * 5;

		bool old = oldgamepad[padNo].rgdwPOV[0] == 4500 * 3 ||
			oldgamepad[padNo].rgdwPOV[0] == 4500 * 4 ||
			oldgamepad[padNo].rgdwPOV[0] == 4500 * 5;

		return !now && old;
	}

	return false;
}

int ControllerInput::IsPadStick(const size_t& stickAxis, const float& deadzone, const size_t& padNo)
{
	if (padNo >= gamepad.size())
	{
		return 0;
	}

	static int deadMin = 0;
	static int deadMax = 0;
	static int stick = 0;

	deadMin = zoneMin <= 0 ? (int)(zoneMin * deadzone) : (int)(zoneMin * deadzone) + zoneMin;
	deadMax = zoneMax >= 0 ? (int)(zoneMax * deadzone) : (int)(zoneMax * deadzone) + zoneMax;

	switch (stickAxis)
	{
		case INPUT_AXIS_LX:
			stick = gamepad[padNo].lX;
			break;
		case INPUT_AXIS_LY:
			stick = gamepad[padNo].lY;
			break;
		case INPUT_AXIS_LZ:
			stick = gamepad[padNo].lZ;
			break;
		case INPUT_AXIS_RX:
			stick = gamepad[padNo].lRx;
			break;
		case INPUT_AXIS_RY:
			stick = gamepad[padNo].lRy;
			break;
		case INPUT_AXIS_RZ:
			stick = gamepad[padNo].lRz;
			break;
		default:
			stick = 0;
			break;
	}

	if (stick <= deadMin && stick < 0 || stick >= deadMax && stick>0)
	{
		return stick;
	}
	else
	{
		return 0;
	}
}

HRESULT ControllerInput::CreateGamepadDevice()
{
	HRESULT result;
	static vector<DIDEVICEINSTANCE> parameter;

	//ゲームパッドデバイス列挙
	result = dinput->EnumDevices(DI8DEVTYPE_GAMEPAD, &EnumGamepadCallback, &parameter, DIEDFL_ATTACHEDONLY);

	for (size_t i = 0; i < parameter.size(); i++)
	{
		devGamepad.push_back({});

		//ゲームパッドデバイスの生成
		result = dinput->CreateDevice(parameter[i].guidInstance, &devGamepad[i], NULL);
		//入力データ形式のセット
		result = devGamepad[i]->SetDataFormat(&c_dfDIJoystick);
		//プロパティの設定
		result = SetupGamepadProperty(i);
		//協調レベルの設定
		result = devGamepad[i]->SetCooperativeLevel(WindowsAPI::hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	}

	return S_OK;
}

BOOL ControllerInput::EnumGamepadCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	vector<DIDEVICEINSTANCE>* ptr = (vector<DIDEVICEINSTANCE>*)pvRef;
	DIDEVICEINSTANCE tmp = *lpddi;

	//配列に格納
	ptr->push_back(tmp);

	return DIENUM_CONTINUE;
}

HRESULT ControllerInput::SetupGamepadProperty(const size_t& padNo)
{
	if (padNo >= devGamepad.size())
	{
		return S_FALSE;
	}

	HRESULT result;

	//軸モードを絶対値モードとして設定
	DIPROPDWORD diprop;
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;
	result = devGamepad[padNo]->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if (FAILED(result))
	{
		return result;
	}

	//X軸の値の範囲設定（LEFT）
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.diph.dwObj = DIJOFS_X;
	diprg.lMin = zoneMin;
	diprg.lMax = zoneMax;
	result = devGamepad[padNo]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(result))
	{
		return result;
	}

	//Y軸の値の範囲設定（LEFT）
	diprg.diph.dwObj = DIJOFS_Y;
	result = devGamepad[padNo]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(result))
	{
		return result;
	}

	//Z軸の値の範囲設定（LEFT、LT＆RT設定）
	diprg.diph.dwObj = DIJOFS_Z;
	result = devGamepad[padNo]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(result))
	{
		return result;
	}


	//RX軸の値の範囲設定（RIGHT）
	diprg.diph.dwObj = DIJOFS_RX;
	result = devGamepad[padNo]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(result))
	{
		return result;
	}

	//RY軸の値の範囲設定（RIGHT）
	diprg.diph.dwObj = DIJOFS_RY;
	result = devGamepad[padNo]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(result))
	{
		return result;
	}

	//RZ軸の値の範囲設定（RIGHT）
	diprg.diph.dwObj = DIJOFS_RZ;
	result = devGamepad[padNo]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(result))
	{
		return result;
	}

	return S_OK;
}

