#pragma once
#ifndef _INPUT_H_
#define _INPUT_H_

#include <vector>

class Input
{
public:
	Input() = default;
	~Input() = default;

	void Init();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

private:
	
	std::vector<bool> keys;
};

#endif