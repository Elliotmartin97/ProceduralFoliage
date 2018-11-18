#include "Bush.h"

Bush::Bush()
{

}

Bush::~Bush()
{

}

void Bush::Generate(ID3D11Device* device, ID3D11DeviceContext* device_context)
{
	L_system = new LSystem;
	L_system->GenerateSystem();

	std::string test = "+[+LFXYF[]+LFXXZF[";

	XMFLOAT3 pos = { 0, 0, 0 };
	XMFLOAT4 rot = { 0, 0, 0, 1 };
	XMFLOAT3 scale = { 0.1, 1, 0.1 };
	int model_count = -1;
	int selected = -1;
	for (int iter = 0; iter < 1; iter++)
	{
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
			case '-':
				
				break;
			case 'X':
				rot.x += 20;
				break;
			case 'Y':
				rot.y += 20;
				break;
			case 'Z':
				rot.z += 20;
				break;
			case 'F':
				current_model->MoveUP(1);
				XMStoreFloat3(&pos,current_model->GetPosition());
				break;
			case '[':
				model_list.push_back(current_model);
				model_count++;
				selected++;
				break;
			case ']':
				selected--;
				current_model = model_list[selected];
				XMStoreFloat3(&pos, current_model->GetPosition());
				XMStoreFloat4(&rot, current_model->GetRotation());
				XMStoreFloat3(&scale, current_model->GetScale());
				break;
			case 'R':
				//scale radius down
				break;
			case 'L':
				scale.y -= 0.25f;
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