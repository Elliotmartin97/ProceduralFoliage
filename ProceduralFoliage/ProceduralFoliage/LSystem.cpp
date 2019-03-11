#include "LSystem.h"
#include <fstream>

LSystem::LSystem()
{
	rules.resize(3);
	//Structure rules

	rules[0].a = 'A';
	rules[0].b = "+LRFXYZF&[^]BC";

	rules[1].a = 'B';
	rules[1].b = "+LRFXYZF&[^BC]+LRFXYZF&[^BC";

	rules[2].a = 'C';
	rules[2].b = "@";

	axiom = "+&[AAAAAAA";
	iterations = 6;
}

LSystem::~LSystem()
{
	TwDeleteBar(tweak_bar);
}

void TW_CALL CopyStdStringToClient2(std::string& destinationClientString, const std::string& sourceLibraryString)
{
	destinationClientString = sourceLibraryString;
}

void TW_CALL SaveCB(void* client_data)
{
	LSystem* lsystem = static_cast<LSystem*>(client_data);

	lsystem->SaveType("");
}

LSystem::LSystem(std::string file_name)
{
	std::ifstream file;
	file.open("../Engine/Lsystems/Types/" + file_name + ".txt");
	file >> type_name;
	file >> rule_size;
	file >> iterations;
	file >> starting_branches;
	file >> fixed_branches;
	file >> leaves;
	file >> show_axiom;
	file >> axiom;
	for (int i = 0; i < starting_branches; i++)
	{
		axiom += "A";
	}
	
	rules.resize(rule_size);
	for (int i = 0; i < rule_size; i++)
	{
		file >> rules[i].a;
		file >> rules[i].b;
	}

	rules[1].b = "+LRFXYZF&[^BC";
	file >> split_count;
	for (int i = 0; i < split_count; i++)
	{
		rules[1].b += "]+LRFXYZF&[^BC";
	}

	file >> scale.x >> scale.y >> scale.z;
	file >> scale_decrease.x >> scale_decrease.y >> scale_decrease.z;
	file >> rotation_range.x >> rotation_range.y >> rotation_range.z;
	file >> branch_rotation.x >> branch_rotation.y >> branch_rotation.z;
	file >> curve;
	file >> leaf_scale_multi.x >> leaf_scale_multi.y;
	file.close();

	//The great anttweakbar wall of text

	const char* bar_name = file_name.c_str();
	TwCopyStdStringToClientFunc(CopyStdStringToClient2);
	tweak_bar = TwNewBar(bar_name);
	TwAddVarRW(tweak_bar, "Type Name", TW_TYPE_STDSTRING, &type_name, "");
	TwAddVarRW(tweak_bar, "Iterations", TW_TYPE_INT32, &iterations, " min= 0 max= 20 ");
	TwAddSeparator(tweak_bar, "", NULL);
	TwAddVarRW(tweak_bar, "Start Branches", TW_TYPE_INT32, &starting_branches, " min= 0 max= 10 ");
	TwAddSeparator(tweak_bar, "", NULL);
	TwAddVarRW(tweak_bar, "Fixed Branches", TW_TYPE_INT32, &fixed_branches, " min= 0 max= 1");
	TwAddSeparator(tweak_bar, "", NULL);
	TwAddVarRW(tweak_bar, "Leaves", TW_TYPE_INT32, &leaves, " min= 0 max= 1");
	TwAddSeparator(tweak_bar, "", NULL);
	TwAddVarRW(tweak_bar, "Show Axiom", TW_TYPE_INT32, &show_axiom, " min= 0 max= 1");
	TwAddSeparator(tweak_bar, "", NULL);
	TwAddVarRW(tweak_bar, "Split Count", TW_TYPE_INT32, &split_count, " min= 0 max= 5");
	TwAddSeparator(tweak_bar, "", NULL);
	TwAddVarRW(tweak_bar, "Scale X", TW_TYPE_FLOAT, &scale.x, "");
	TwAddVarRW(tweak_bar, "Scale Y", TW_TYPE_FLOAT, &scale.y, "");
	TwAddVarRW(tweak_bar, "Scale Z", TW_TYPE_FLOAT, &scale.z, "");
	TwAddSeparator(tweak_bar, "", NULL);
	TwAddVarRW(tweak_bar, "Scale Shrink X", TW_TYPE_FLOAT, &scale_decrease.x, "");
	TwAddVarRW(tweak_bar, "Scale Shrink Y", TW_TYPE_FLOAT, &scale_decrease.y, "");
	TwAddVarRW(tweak_bar, "Scale Shrink Z", TW_TYPE_FLOAT, &scale_decrease.z, "");
	TwAddSeparator(tweak_bar, "", NULL);
	TwAddVarRW(tweak_bar, "Rotate Range X", TW_TYPE_FLOAT, &rotation_range.x, "");
	TwAddVarRW(tweak_bar, "Rotate Range Y", TW_TYPE_FLOAT, &rotation_range.y, "");
	TwAddVarRW(tweak_bar, "Rotate Range Z", TW_TYPE_FLOAT, &rotation_range.z, "");
	TwAddSeparator(tweak_bar, "", NULL);
	TwAddVarRW(tweak_bar, "Branch Rotation X", TW_TYPE_FLOAT, &branch_rotation.x, "");
	TwAddVarRW(tweak_bar, "Branch Rotation Y", TW_TYPE_FLOAT, &branch_rotation.y, "");
	TwAddVarRW(tweak_bar, "Branch Rotation Z", TW_TYPE_FLOAT, &branch_rotation.z, "");
	TwAddSeparator(tweak_bar, "", NULL);
	TwAddVarRW(tweak_bar, "Curve", TW_TYPE_FLOAT, &curve, " min= 0.0 max= 1.0");
	TwAddSeparator(tweak_bar, "", NULL);
	TwAddVarRW(tweak_bar, "Leaf X", TW_TYPE_FLOAT, &leaf_scale_multi.x, " min= 0.0 max= 2.0");
	TwAddVarRW(tweak_bar, "Leaf Y", TW_TYPE_FLOAT, &leaf_scale_multi.y, " min= 0.0 max= 2.0");
	TwAddSeparator(tweak_bar, "", NULL);
	TwAddSeparator(tweak_bar, "", NULL);

	TwAddButton(tweak_bar, "SAVE", SaveCB, this, " label='SAVE TYPE ");

}

void LSystem::SaveType(std::string name)
{
	std::ofstream file;
	if (name == "")
	{
		file.open("../Engine/Lsystems/Types/" + type_name + ".txt");
	}
	else
	{
		file.open("../Engine/Lsystems/Types/" + name + ".txt");
	}
	file.clear();
	file << type_name << std::endl;
	file << rule_size << std::endl;
	file << iterations << std::endl;
	file << starting_branches << std::endl;
	file << fixed_branches << std::endl;
	file << leaves << std::endl;
	file << show_axiom << std::endl;
	axiom = "+&[";
	file << axiom << std::endl;

	rules.resize(rule_size);
	for (int i = 0; i < rule_size; i++)
	{
		file << rules[i].a << std::endl;
		file << rules[i].b << std::endl;
	}

	file << split_count << std::endl;


	file << scale.x << std::endl << scale.y << std::endl << scale.z << std::endl;
	file << scale_decrease.x << std::endl << scale_decrease.y << std::endl << scale_decrease.z << std::endl;
	file << rotation_range.x << std::endl << rotation_range.y << std::endl << rotation_range.z << std::endl;
	file << branch_rotation.x << std::endl << branch_rotation.y << std::endl << branch_rotation.z << std::endl;
	file << curve << std::endl;
	file << leaf_scale_multi.x << std::endl << leaf_scale_multi.y;
	file.close();
}

void LSystem::GenerateSystem()
{
	std::ofstream file;
	file.open("../Engine/LSystems/data.txt");
	file.clear();
	std::string next_sentence = axiom;

	for (int i = 0; i < iterations; i++)
	{
		data.push_back(next_sentence);
		file << next_sentence << std::endl;
		std::string current_sentence = next_sentence;
		next_sentence.clear();
		next_sentence = "#";
		for (int j = 0; j < current_sentence.size(); j++)
		{
			bool found = false;
			for (int x = 0; x < rules.size(); x++)
			{
				if (current_sentence[j] == rules[x].a)
				{
					next_sentence += rules[x].b;
					found = true;
					break;
				}
			}
			if (!found)
			{
				next_sentence += "";
			}
		}
	}

	file.close();
}

bool LSystem::IsFixed()
{
	if (fixed_branches == 1)
	{
		return true;
	}
	return false;
}

bool LSystem::ShowLeaves()
{
	if (leaves == 1)
	{
		return true;
	}
	return false;
}

bool LSystem::ShowAxiom()
{
	if (show_axiom == 1)
	{
		return true;
	}
	return false;
}
