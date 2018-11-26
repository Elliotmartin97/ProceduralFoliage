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

	XMFLOAT3 pos = { 0, 0, 0 };
	XMFLOAT4 rot = { 0, 0, 0, 1 };
	XMFLOAT3 scale = { 0.1f, 1, 0.1f };
	int selected = 0;
	float r;
	Model* previous_model = nullptr;

	std::vector<Model*> previous_list;
	std::vector<Model*> current_list;

	for (int iter = 0; iter < L_system->GetIterations(); iter++)
	{
		std::string test = L_system->GetData(iter);
		current_list.clear();
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
				r = (3.141592654f * 2.0f * ((float)rand() / (float)RAND_MAX));// (float)rand() / ((float)RAND_MAX / 2.0f) - 1.0f;
				rot.x += r;
				break;
			case 'Y':
				r = (3.141592654f * 2.0f * ((float)rand() / (float)RAND_MAX));// (float)rand() / ((float)RAND_MAX / 2.0f) - 1.0f;
				rot.y += r;
				break;
			case 'Z':
				r = (3.141592654f * 2.0f * ((float)rand() / (float)RAND_MAX));// (float)rand() / ((float)RAND_MAX / 2.0f) - 1.0f;
				rot.z += r;
				break;
			case 'F':
				current_model->MoveUP(1);
				XMStoreFloat3(&pos,current_model->GetPosition());
				break;
			case '[':
				current_list.push_back(current_model);
				current_model = nullptr;
				break;
			case ']':
				previous_model = previous_list[selected];
				XMStoreFloat3(&pos, previous_model->GetPosition());
				XMStoreFloat4(&rot, previous_model->GetRotation());
				XMStoreFloat3(&scale, previous_model->GetScale());
				break;
			case '@':
				if (selected < previous_list.size() - 1)
				{
					selected++;
				}
				previous_model = previous_list[selected];
				XMStoreFloat3(&pos, previous_model->GetPosition());
				XMStoreFloat4(&rot, previous_model->GetRotation());
				XMStoreFloat3(&scale, previous_model->GetScale());
				break;
			case '#':
				selected = 0;
				if (previous_model)
				{
					previous_model = previous_list[selected];
					XMStoreFloat3(&pos, previous_model->GetPosition());
					XMStoreFloat4(&rot, previous_model->GetRotation());
					XMStoreFloat3(&scale, previous_model->GetScale());
				}
				break;
			case 'R':
				scale.x /= 1.5f;
				scale.z /= 1.5f;
				break;
			case 'L':
				scale.y /= 1.25f;
				break;
			}
			if (current_model)
			{
				current_model->SetPosition(pos.x, pos.y, pos.z);
				current_model->SetRotation(rot.x, rot.y, rot.z, 1.0f);
				current_model->SetScale(scale.x, scale.y, scale.z);
			}
		}
		previous_list = current_list;
		render_list.insert(render_list.end(), current_list.begin(), current_list.end());
		current_list.clear();
		selected = 0;
	}
}