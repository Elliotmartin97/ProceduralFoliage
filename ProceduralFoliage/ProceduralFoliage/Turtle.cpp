#include "Turtle.h"
#include <time.h>
Turtle::Turtle()
{

}

Turtle::~Turtle()
{

}

void Turtle::Generate(ID3D11Device* device, ID3D11DeviceContext* device_context, std::string name)
{
	srand(time(NULL));
	L_system = new LSystem(name);
	L_system->GenerateSystem();

	XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
	XMFLOAT4 rot = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMFLOAT3 scale = L_system->GetStartScale();
	XMFLOAT3 rotation_range = L_system->GetRange();
	XMFLOAT3 branch_rotation = L_system->GetBranchRot();

	Model* previous_model = nullptr;
	std::vector<Model*> previous_list;
	std::vector<Model*> current_list;
	std::vector<Model*> leaves_list;
	float curve = L_system->GetCurve();
	bool fixed_branches = L_system->IsFixed();

	float range = 0.0f;
	int selected = 0;
	int counter = 0;

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
				//flip the x to get branches on either side of foliage
				branch_rotation.x = -branch_rotation.x;
				current_model = new Model;
				current_model->Init(device, device_context, (char*)"../Engine/Models/Cube.txt", (char*)"../Engine/Textures/Green.tga");
				counter++;
			}
				break;
			case 'X':

				if (fixed_branches)
				{
					//only for starting branches
					if (rot.x == 0.0f)
					{

						range = (rotation_range.x * ((float)rand() / (float)RAND_MAX));

						if (branch_rotation.x > 0.0f)
						{
							rot.x = branch_rotation.x - range;
						}
						else
						{
							rot.x = branch_rotation.x + range;
						}

					}
					else
					{
						if (rot.x > 0.0f)
						{
							rot.x += curve;
						}
						else
						{
							rot.x -= curve;
						}
					}
				}
				else
				{
					range = (rotation_range.x * ((float)rand() / (float)RAND_MAX));
					rot.x = branch_rotation.x + range;
				}
				
				break;
			case 'Y':
				if (fixed_branches)
				{

				}
				else
				{
					range = (rotation_range.y * ((float)rand() / (float)RAND_MAX));
					rot.y = branch_rotation.y + range;
				}
				break;
			case 'Z':
				if (fixed_branches)
				{
					//only for starting branches
					if (rot.z == 0.0f)
					{
						float range = (rotation_range.z * ((float)rand() / (float)RAND_MAX));
						rot.z = branch_rotation.z + range;
					}
				}
				else
				{
					float range = (rotation_range.z * ((float)rand() / (float)RAND_MAX));
					rot.z = branch_rotation.z + range;
				}
				break;
			case 'F':
				current_model->MoveUp(1);
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
				scale.x /= L_system->GetScaleDecrease().x;
				scale.z /= L_system->GetScaleDecrease().z;
				break;
			case 'L':
				scale.y /= L_system->GetScaleDecrease().y;
				break;
			case '*':
				scale.y *= 1.25f;
				break;
			case '^':
				Model* leave_model = new Model;
				leave_model->Init(device, device_context, (char*)"../Engine/Models/Quad.txt", (char*)"../Engine/Textures/Green.tga");
				leave_model->SetPosition(pos.x, pos.y, pos.z);
				leave_model->SetRotation(rot.x, rot.y, rot.z, 1.0f);
				leave_model->SetScale(scale.y, scale.y, 1.0f);
				leaves_list.push_back(leave_model);
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
		render_list.insert(render_list.end(), leaves_list.begin(), leaves_list.end());
		current_list.clear();
		selected = 0;
	}
	previous_list.clear();
	int z = counter;
}