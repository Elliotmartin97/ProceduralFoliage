#pragma once
#include <string>
#include <vector>
#include "Direct3D.h"
struct Rules
{
	char a;
	std::string b;
};

class LSystem
{

public:
	LSystem();
	LSystem(std::string file_name);
	~LSystem() = default;

	void GenerateSystem();
	std::string GetData(int index) { return data[index]; };
	int GetIterations() { return iterations; };
	XMFLOAT3 GetStartScale() { return start_scale; };
	XMFLOAT3 GetScaleDecrease() { return scale_decrease; };
	XMFLOAT3 GetRange() { return rotation_range; };
	XMFLOAT3 GetBranchRot() { return branch_rotation; };
	float GetCurve() { return curve; };
	bool IsFixed();

private:
	std::vector<std::string> data;
	int split_count = 0;
	int fixed_branches = 0;
	std::string axiom;
	std::vector<Rules> rules;
	int starting_branches = 1;
	int rule_size;
	int iterations;
	XMFLOAT3 start_scale;
	XMFLOAT3 scale_decrease;
	XMFLOAT3 rotation_range;
	XMFLOAT3 branch_rotation;

	float curve = 0.0f;
	
};