#pragma once
#include "Direct3D.h"
#include "Model.h"
#include "LSystem.h"

class FoliageBase {
public:
	 FoliageBase() = default;
	 ~FoliageBase() = default;

	 virtual void Generate(ID3D11Device* device, ID3D11DeviceContext* device_context) = 0;
	 std::vector<Model*> GetModelList() { return model_list; };
protected:
	std::vector<Model*> model_list;
	LSystem* L_system;
};