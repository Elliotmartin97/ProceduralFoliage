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

	std::string test = "+[+FX[+F[";

	float rot_x = 0, rot_y = 0, rot_z = 0;
	float pos_x = 0, pos_y = 0, pos_z = 0;
	float sca_x = 0.1, sca_y = 1, sca_z = 0.1;

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
				rot_x += 20;
				break;
			case 'Y':
				rot_y += 20;
				break;
			case 'Z':
				rot_z += 20;
				break;
			case 'F':
				pos_y += 1;
				current_model->SetVertexData(FACE_TOP, 1);
				current_model->SetVertexData(FACE_BOTTOM, 0);
				break;
			case '[':
				model_list.push_back(current_model);
				break;
			case ']':
				//pop
				break;
			case 'R':
				//scale radius down
				break;
			case 'L':
				//scale length down
				break;
			}
			if (current_model)
			{
				current_model->SetRotation(rot_x, rot_y, rot_z);
				current_model->SetPosition(pos_x, pos_y, pos_z);
				current_model->SetScale(sca_x, sca_y, sca_z);
			}
		}
	}
}