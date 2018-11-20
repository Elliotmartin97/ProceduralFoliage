#include "Bush.h"
#include <time.h>
Bush::Bush()
{

}

Bush::~Bush()
{

}

void Bush::Generate(ID3D11Device* device, ID3D11DeviceContext* device_context)
{
	srand(time(NULL));
	L_system = new LSystem;
	L_system->GenerateSystem();
	float r;
	//std::string test = "+]LRFXYZF[+]LRFXYZF[";
	std::string test = L_system->GetData(1);

	XMFLOAT3 pos = { 0, 0, 0 };
	XMFLOAT4 rot = { 0, 0, 0, 1 };
	XMFLOAT3 scale = { 0.1, 1, 0.1 };
	int selected = 1;
	Model* previous_model;

	current_model = new Model;
	current_model->Init(device, device_context, (char*)"../Engine/Models/Cube.txt", (char*)"../Engine/Textures/Green.tga");
	model_list.push_back(current_model);
	if (current_model)
	{
		current_model->SetPosition(pos.x, pos.y, pos.z);
		current_model->SetRotation(rot.x, rot.y, rot.z, 1.0f);
		current_model->SetScale(scale.x, scale.y, scale.z);
	}

	for (int iter = 0; iter < L_system->GetIterations(); iter++)
	{
		std::string test = L_system->GetData(iter);
		for (int i = 0; i < test.size(); i++)
		{
			switch (test[i])
			{
			case '+':
			{
				current_model = new Model;
				current_model->Init(device, device_context, (char*)"../Engine/Models/Cube.txt", (char*)"../Engine/Textures/Green.tga");
			}
				break;
			case 'X':
				r = rand() % 360;
				rot.x += r;
				break;
			case 'Y':
				r = rand() % 360;
				rot.y += r;
				break;
			case 'Z':
				r = rand() % 360;
				rot.z += r;
				break;
			case 'F':
				current_model->MoveUP(1);
				XMStoreFloat3(&pos,current_model->GetPosition());
				break;
			case '[':
				model_list.push_back(current_model);
				selected++;
				break;
			case ']':
				selected--;
				previous_model = model_list[selected];
				XMStoreFloat3(&pos, previous_model->GetPosition());
				XMStoreFloat4(&rot, previous_model->GetRotation());
				XMStoreFloat3(&scale, previous_model->GetScale());
				break;
			case 'R':
				scale.x /= 1.5f;
				scale.z /= 1.5f;
				break;
			case 'L':
				scale.y /= 1.5f;
				break;
			}
			if (current_model)
			{
				current_model->SetPosition(pos.x, pos.y, pos.z);
				current_model->SetRotation(rot.x, rot.y, rot.z, 1.0f);
				current_model->SetScale(scale.x, scale.y, scale.z);
			}
		}
	}
}