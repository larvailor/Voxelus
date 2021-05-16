#pragma once

class Mouse
{
public:
	Mouse() = delete;

	static double OffsetX;
	static double OffsetY;

	static double LastFrameXpos;
	static double LastFrameYpos;

	static double Sensitivity;

	static int Button;
	static int Action;
	static int Mods;

	static bool mLMB_Pressed;
};