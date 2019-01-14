#include "LSystem.h"
#include <fstream>

LSystem::LSystem()
{
	//default
	rules.resize(3);
	rules[0].a = 'A';
	rules[0].b = "+LRFXYZF[^]BC";

	rules[1].a = 'B';
	rules[1].b = "+LRFXYZF[^BC]+LRFXYZF[^BC";

	rules[2].a = 'C';
	rules[2].b = "@";

	axiom = "+[AAAAAAA";
	iterations = 6;
}

LSystem::LSystem(std::string file_name)
{
	std::ifstream file;
	file.open("../Engine/Lsystems/Types/" + file_name + ".txt");
	file >> rule_size;
	file >> iterations;
	file >> starting_branches;
	file >> fixed_branches;
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

	file >> split_count;
	for (int i = 0; i < split_count; i++)
	{
		rules[1].b += "]+LRFXYZF[^BC";
	}

	file >> start_scale.x >> start_scale.y >> start_scale.z;
	file >> scale_decrease.x >> scale_decrease.y >> scale_decrease.z;
	file >> rotation_range.x >> rotation_range.y >> rotation_range.z;
	file >> branch_rotation.x >> branch_rotation.y >> branch_rotation.z;
	file >> curve;
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
