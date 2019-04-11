#include "Input.h"

void Input::Init()
{
	keys.resize(256);
	// Initialize all the keys to being released and not pressed.
	for (int i = 0; i < 256; i++)
	{
		keys[i] = false;
	}
}

void Input::KeyDown(unsigned int input)
{
	keys[input] = true;
}

void Input::KeyUp(unsigned int input)
{
	keys[input] = false;
}

bool Input::IsKeyDown(unsigned int key)
{
	return keys[key];
}