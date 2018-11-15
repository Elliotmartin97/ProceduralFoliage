#pragma once
#include <vector>

#include "FoliageBase.h"
#include "Model.h"

class Bush : public FoliageBase {

public:
	Bush();
	~Bush();

	void Generate(ID3D11Device* device, ID3D11DeviceContext* device_context) override;
private:
	std::vector<Model*> branch_list;
	Model* current_model;
};