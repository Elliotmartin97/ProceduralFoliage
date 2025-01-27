#pragma once
#include <vector>
#include "LSystem.h"
#include "Model.h"

class Turtle {

public:
	Turtle();
	~Turtle();

	void Generate(ID3D11Device* device, ID3D11DeviceContext* device_context, std::string name);
	LSystem* GetLSystem() { return L_system; };
	std::string GetName() { return L_system->GetName(); };
	std::vector<Model*> GetModelList() { return render_list; };
private:
	std::vector<Model*> render_list;
	LSystem* L_system;
	std::vector<Model*> branch_list;
	Model* current_model = nullptr;
	TwBar* tweak_bar;
};