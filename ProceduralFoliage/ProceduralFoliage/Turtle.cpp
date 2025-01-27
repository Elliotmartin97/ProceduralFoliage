#include "Turtle.h"
#include <time.h>
Turtle::Turtle()
{

}

Turtle::~Turtle()
{
	delete L_system;
	L_system = nullptr;

	for (int i = 0; i < render_list.size(); i++)
	{
		render_list[i]->Shutdown();
	}
}

void Turtle::Generate(ID3D11Device* device, ID3D11DeviceContext* device_context, std::string name)
{
	srand(time(NULL));
	L_system = new LSystem(name);
	L_system->GenerateSystem();

	XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
	XMFLOAT4 rot = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMFLOAT3 scale =  L_system->GetStartScale();
	XMFLOAT3 rotation_range = L_system->GetRange();
	XMFLOAT3 branch_rotation = L_system->GetBranchRot();
	XMFLOAT2 leaf_multi = L_system->GetLeafScale();

	Model* previous_model = nullptr;
	std::vector<Model*> previous_list;
	std::vector<Model*> current_list;
	std::vector<Model*> leaves_list;
	float curve = L_system->GetCurve();
	bool fixed_branches = L_system->IsFixed();

	float range = 0.0f;
	int selected = 0;
	int counter = 0;
	bool do_once = true;

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
				if (fixed_branches)
				{
					branch_rotation.x = -branch_rotation.x;
				}
				current_model = new Model;
				current_model->Init(device, device_context, (char*)"../Engine/Models/Cube.txt", (char*)"../Engine/Textures/branch.tga", (char*)"../Engine/Textures/branch.tga", (char*)"../Engine/Textures/branch.tga");
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
				//Rotate the model of the Y Axis of its transform
				if (!fixed_branches)
				{
					range = (rotation_range.y * ((float)rand() / (float)RAND_MAX));
					rot.y = branch_rotation.y + range;
				}
				break;
			case 'Z':
				//rotate the model of the Z axis of its transform
				if (fixed_branches)
				{
					//only use random values for starting branches, the rest are in fixed positions
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
				//move the model forward on its transform
				current_model->MoveUp(1.1f);
				XMStoreFloat3(&pos,current_model->GetPosition());
				break;
			case '[':
				//add the current model to the overall list before creating a new one
				current_list.push_back(current_model);
				current_model = nullptr;
				break;
			case ']':
				//get the previous model to gain access to its data such as position, rotation and scale
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
				if (L_system->ShowLeaves())
				{
					Model* leaf_model = new Model;
					leaf_model->Init(device, device_context, (char*)"../Engine/Models/Quad.txt", (char*)"../Engine/Textures/leaf.tga", (char*)"../Engine/Textures/leaf.tga", (char*)"../Engine/Textures/leaf.tga");
					leaf_model->SetBlendAmount(1.0f);
					leaf_model->SetPosition(pos.x, pos.y, pos.z);
					leaf_model->SetRotation(rot.x, rot.y, rot.z, 1.0f);
					leaf_model->SetScale(scale.y * -leaf_multi.x, scale.y * leaf_multi.y, 1.0f);
					leaf_model->TransformVertexData();
					//leaf_model->SetBufferToTransformedVertices(device);
					leaves_list.push_back(leaf_model);
				}
				break;
			case '&':
				//fuse the ends of two models together
				if (previous_model && current_model)
				{
					if (L_system->ShowAxiom() == false)
					{
						if (previous_model == render_list[0])
						{
							current_model->TransformVertexData();
							current_model->SetBufferToTransformedVertices(device);
							break;
						}
					}
					current_model->TransformVertexData();
					for (int i = 0; i < current_model->GetVertexCount(); i++)
					{
						current_model->LinkBotPosition(previous_model, i);
					}
					current_model->SetBufferToTransformedVertices(device);
				}
				else //set the transformed data and break
				{
					current_model->TransformVertexData();
					current_model->SetBufferToTransformedVertices(device);
				}
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
	render_list.insert(render_list.end(), leaves_list.begin(), leaves_list.end());
	previous_list.clear();
	int z = counter;
	if (L_system->ShowAxiom() != true)
	{
		render_list[0]->Shutdown();
		render_list.erase(render_list.begin());
	}
}