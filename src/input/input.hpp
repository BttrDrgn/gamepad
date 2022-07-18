#pragma once

class input final
{
public:
	static void init();
	static void update();

	static SDL_Joystick* joystick;
	static float gun_angle;
	static float sensitivity;
	static bool ready;
	static bool shoot;
	static bool fast_forward;

private:
	static void determine_buttons();
	static void click();
	static bool has_shot;
	static POINT cursor_pos;
};
