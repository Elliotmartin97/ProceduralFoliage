#pragma once


#ifndef _MODELEXPORTER_H_
#define _MODELEXPORTER_H_

#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <string>

#include "Model.h"


class ModelExporter 
{
public:

	ModelExporter();
	ModelExporter(const ModelExporter&);
	~ModelExporter();
	bool GenerateMTL(std::string name);
	bool Export(std::string name, std::vector<Model*> model_list, XMMATRIX world_matrix);
	void Shutdown();
private:
	ModelType* model_type;
	std::vector<std::string> MTL_list;
};
#endif