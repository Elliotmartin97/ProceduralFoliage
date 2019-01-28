#include "ModelExporter.h"

ModelExporter::ModelExporter()
{
}

ModelExporter::ModelExporter(const ModelExporter&)
{
}

ModelExporter::~ModelExporter()
{
}

void ModelExporter::Shutdown()
{
	if (model_type)
	{
		delete[] model_type;
		model_type = 0;
	}
}

bool ModelExporter::Export(std::string name, std::vector<Model*> model_list, XMMATRIX world_matrix)
{
	std::string txt_file = "../Engine/Obj/" + name + ".obj";
	fstream file;
	file.open(txt_file, std::ofstream::out | std::ofstream::trunc);
	int face_counter = 0;
	int model_counter = 0;

	if (file.fail())
	{
		return false;
	}

	file << "# Created from ModelExporter using DirectX11" << std::endl;
	file << "# MTL EXTERNALS" << std::endl;

	//Create MTL'S
	for (int i = 0; i < model_list.size(); i++)
	{

		if (GenerateMTL(model_list[i]->GetTextureName()))
		{
			file << "mtllib " << model_list[i]->GetTextureName() << ".mtl" << std::endl;
		}
		
	}

	file << std::endl << "# Vertices" << std::endl;

	for (int i = 0; i < model_list.size(); i++)
	{
		int vert_count;
		vert_count = model_list[i]->GetVertexCount();

		model_type = new ModelType(*model_list[i]->GetModelType());
		if (!model_type)
		{
			return false;
		}

		//transform the vertex data by world matrix before exporting
		for (int v = 0; v < vert_count; v++)
		{
			FXMVECTOR temp_vec = XMVectorSet(model_list[i]->GetModelType()[v].x, model_list[i]->GetModelType()[v].y, model_list[i]->GetModelType()[v].z, 1.0f);
			XMVECTOR transformed_vec = XMVector3Transform(temp_vec, model_list[i]->GetTransform());
			XMFLOAT3* new_vertices = new XMFLOAT3;
			XMStoreFloat3(new_vertices, transformed_vec);

			file << "v" << " " <<
				new_vertices->x << " " <<
				new_vertices->y << " " <<
				new_vertices->z << " " <<
				std::endl;

		}
	}

	file << std::endl << "# Textures" << std::endl;

	for (int i = 0; i < model_list.size(); i++)
	{

		int vert_count;
		vert_count = model_list[i]->GetVertexCount();

		model_type = new ModelType(*model_list[i]->GetModelType());
		if (!model_type)
		{
			return false;
		}

		for (int v = 0; v < vert_count; v++)
		{
			file << "vt" << " " <<
				model_list[i]->GetModelType()[v].tu << " " <<
				model_list[i]->GetModelType()[v].tv <<
				std::endl;
		}

	}

	file << std::endl << "# Normals" << std::endl;

	for (int i = 0; i < model_list.size(); i++)
	{
		int vert_count;
		vert_count = model_list[i]->GetVertexCount();

		model_type = new ModelType(*model_list[i]->GetModelType());
		if (!model_type)
		{
			return false;
		}

		for (int v = 0; v < vert_count; v++)
		{
			file << "vn" << " " <<
				model_list[i]->GetModelType()[v].nx << " " <<
				model_list[i]->GetModelType()[v].ny << " " <<
				model_list[i]->GetModelType()[v].nz << " " <<
				std::endl;
		}
	}

	file << std::endl << "# Faces" << std::endl;

	for (int i = 0; i < model_list.size(); i++)
	{
		model_counter++;
		int vert_count;
		vert_count = model_list[i]->GetVertexCount();

		model_type = new ModelType(*model_list[i]->GetModelType());
		if (!model_type)
		{
			return false;
		}

		file << std::endl << "g model " << model_counter << std::endl;
		file << "usemtl "<< model_list[i]->GetTextureName() << std::endl;

		int counter = 3;
		for (int v = 1; v < vert_count + 1; v++)
		{
			face_counter++;
			if (counter == 3)
			{
				counter = 0;
				file << std::endl << "f" << " ";
			}
			file << face_counter << "/" << face_counter << "/" << face_counter << " ";
			counter++;
		}	
	}

	file.close();

	return true;
}

bool ModelExporter::GenerateMTL(std::string name)
{
	for (int i = 0; i < MTL_list.size(); i++)
	{
		//if theres already an mtl file for the texture then back out
		if (MTL_list[i] == name)
		{
			return false;
		}
	}

	std::string txt_file = "../Engine/Obj/" + name + ".mtl";

	fstream mtl_file;
	mtl_file.open(txt_file, std::ofstream::out | std::ofstream::trunc);

	mtl_file << "newmtl " << name << std::endl;
	mtl_file << "Ka 1.000000 1.000000 1.000000" << std::endl;
	mtl_file << "Kd 1.000000 1.000000 1.000000" << std::endl;
	mtl_file << "Ks 0.000000 0.000000 0.000000" << std::endl;
	mtl_file << "Tr 1.000000" << std::endl;
	mtl_file << "illum 1" << std::endl;
	mtl_file << "Ns 0.000000" << std::endl;
	mtl_file << "map_Kd " << name << ".tga" << std::endl;

	MTL_list.push_back(name);
	return true;
}