#pragma once
#include <string>
#include <vector>
#include "Direct3D.h"
#include "AntTweakBar.h"
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
	~LSystem();

	std::string GetName() { return type_name; };
	void GenerateSystem();
	void SaveType(std::string name);
	std::string GetData(int index) { return data[index]; };
	int GetIterations() { return iterations; };
	XMFLOAT3 GetStartScale() { return scale; };
	XMFLOAT3 GetScaleDecrease() { return scale_decrease; };
	XMFLOAT3 GetRange() { return rotation_range; };
	XMFLOAT3 GetBranchRot() { return branch_rotation; };
	XMFLOAT2 GetLeafScale() { return leaf_scale_multi; };
	float GetCurve() { return curve; };
	bool IsFixed();
	bool ShowLeaves();
	bool ShowAxiom();

private:
	TwBar* tweak_bar;
	std::string type_name;
	std::vector<std::string> data;
	int split_count = 0;
	int fixed_branches = 0;
	int leaves = 0;
	int show_axiom = 1;
	std::string axiom;
	std::vector<Rules> rules;
	int starting_branches = 1;
	int rule_size;
	int iterations;
	XMFLOAT3 scale;
	XMFLOAT3 scale_decrease;
	XMFLOAT3 rotation_range;
	XMFLOAT3 branch_rotation;
	XMFLOAT2 leaf_scale_multi;

	float curve = 0.0f;
	
};