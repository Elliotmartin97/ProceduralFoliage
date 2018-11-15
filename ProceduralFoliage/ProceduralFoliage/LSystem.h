#pragma once
#include <string>
#include <vector>

struct Rules
{
	char a;
	std::string b;
};

class LSystem
{

public:
	LSystem();
	~LSystem() = default;

	void GenerateSystem();
	std::string GetData(int index) { return data[index]; };
	int GetIterations() { return iterations; };
private:
	std::vector<std::string> data;
	std::string axiom;
	std::vector<Rules> rules;
	int iterations;
};