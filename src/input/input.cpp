#include "input.hpp"
#include "logger/logger.hpp"
#include "sdk/SexySDK.hpp"

SDL_Joystick* input::joystick;
float input::gun_angle = 0.0f;
float input::sensitivity = 1.0f;
bool input::ready = false;
bool input::shoot = false;
bool input::has_shot = false;
bool input::fast_forward = false;
POINT input::cursor_pos = { 400, 300 };

static bool(__fastcall* Sexy__WidgetManager__IsRightButtonDown_)(char*, char*);
bool __fastcall Sexy__WidgetManager__IsRightButtonDown(char* this_, char* edx)
{
	auto retn = Sexy__WidgetManager__IsRightButtonDown_(this_, edx);

	if (input::fast_forward)
	{
		retn = true;
	}

	return retn;
}

static int(__fastcall* Sexy__LogicMgr__SetGunAngle_)(Sexy::LogicMgr*, char*, float);
int __fastcall Sexy__LogicMgr__SetGunAngle(Sexy::LogicMgr* this_, char* edx, float angle)
{
	return Sexy__LogicMgr__SetGunAngle_(this_, edx, Sexy::LogicMgr::DegreesToRadians(input::gun_angle));
}

void input::init()
{
	input::joystick = SDL_JoystickOpen(0);
	input::ready = true;
	PRINT_DEBUG("Joystick: %s", SDL_JoystickName(input::joystick));

	MH_Initialize();
	MH_CreateHook((void*)0x0053E370, Sexy__WidgetManager__IsRightButtonDown, (void**)&Sexy__WidgetManager__IsRightButtonDown_);
	MH_CreateHook((void*)0x00436FD0, Sexy__LogicMgr__SetGunAngle, (void**)&Sexy__LogicMgr__SetGunAngle_);
	MH_EnableHook(MH_ALL_HOOKS);
}

void input::update()
{
	if (!input::ready) return;

	//Get states
	input::determine_buttons();

	//Logic

	//Mouse
	HWND hwnd = FindWindowA(0, "Peggle Deluxe 1.01");
	if (hwnd && GetFocus() == hwnd)
	{
		RECT rect = { 0 };
		GetWindowRect(hwnd, &rect);
		SetCursorPos(rect.right - input::cursor_pos.x, rect.bottom - input::cursor_pos.y);
	}

	if (input::shoot && !input::has_shot)
	{
		Sexy::LogicMgr::MouseDown(0, 0, 1, false, false);
		input::click();
		input::has_shot = true;
	}
	else if(!input::shoot && input::has_shot)
	{
		input::has_shot = false;
	}

	if (input::gun_angle > 100.0f)
	{
		input::gun_angle = 100.0f;
	}

	if (input::gun_angle < -100.0f)
	{
		input::gun_angle = -100.0f;
	}

	Sexy::LogicMgr::SetGunAngleDegrees(input::gun_angle);
}

void input::determine_buttons()
{
	//Updates the states
	SDL_JoystickUpdate();

	//Get button number
	/*for (int i = 0; i < 20; ++i)
	{
		if (SDL_JoystickGetButton(input::joystick, i))
		{
			PRINT_DEBUG("%i", i);
		}
	}*/

	//A
	if (SDL_JoystickGetButton(input::joystick, 0))
	{
		input::shoot = true;
	}
	else if (!SDL_JoystickGetButton(input::joystick, 0))
	{
		input::shoot = false;
	}

	//X
	if (SDL_JoystickGetButton(input::joystick, 2))
	{
		input::fast_forward = true;
	}
	else if (!SDL_JoystickGetButton(input::joystick, 2))
	{
		input::fast_forward = false;
	}

	//Right Trigger
	if (SDL_JoystickGetAxis(input::joystick, 4) > 1 || SDL_JoystickGetAxis(input::joystick, 5) > 1)
	{
		input::sensitivity = 0.5f;
	}
	else if (SDL_JoystickGetAxis(input::joystick, 4) <= 0 || SDL_JoystickGetAxis(input::joystick, 5) <= 0)
	{
		input::sensitivity = 1.0f;
	}

	//Left Bumper
	if (SDL_JoystickGetButton(input::joystick, 9))
	{
		input::gun_angle -= 0.05f;
	}

	//Right Bumper
	if (SDL_JoystickGetButton(input::joystick, 10))
	{
		input::gun_angle += 0.05f;
	}

	WORD deadzone = 4860;

	int16_t left_x_motion = SDL_JoystickGetAxis(input::joystick, 0);
	float left_x_step = ((left_x_motion + 0.5) / 32767.5f) * input::sensitivity;

	if (left_x_motion > deadzone || left_x_motion < -deadzone)
	{
		input::gun_angle += left_x_step;
	}

	int16_t right_x_motion = SDL_JoystickGetAxis(input::joystick, 2);
	int16_t right_y_motion = SDL_JoystickGetAxis(input::joystick, 3);
	float right_x_step = (right_x_motion + 0.5) / 32767.5f;
	float right_y_step = (right_y_motion + 0.5) / 32767.5f;

	if (right_x_motion > deadzone || right_x_motion < -deadzone)
	{
		input::cursor_pos.x -= right_x_step * 4.0f;
		if (input::cursor_pos.x > 800)
		{
			input::cursor_pos.x = 800;
		}

		if (input::cursor_pos.x < 2)
		{
			input::cursor_pos.x = 2;
		}
	}

	if (right_y_motion > deadzone || right_y_motion < -deadzone)
	{
		input::cursor_pos.y -= right_y_step * 4.0f;

		if (input::cursor_pos.y > 600)
		{
			input::cursor_pos.y = 600;
		}

		if (input::cursor_pos.y < 2)
		{
			input::cursor_pos.y = 2;
		}
	}
}

//https://cplusplus.com/forum/windows/24162/#msg129336
void input::click()
{
	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &input, sizeof(INPUT));

	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(INPUT));
}
