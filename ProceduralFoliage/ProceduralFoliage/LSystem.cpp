#include "LSystem.h"
#include <fstream>

LSystem::LSystem()
{
	rules.resize(4);
	rules[0].a = 'A';
	rules[0].b = "AB";

	rules[1].a = 'B';
	rules[1].b = "A";

	axiom = 'A';
	iterations = 10;
}

void LSystem::GenerateSystem()
{
	std::ofstream file;
	file.open("../Engine/LSystems/data.txt");
	std::string next_sentence = axiom;

	for (int i = 0; i < iterations; i++)
	{
		data.push_back(next_sentence);
		file << next_sentence << std::endl;
		std::string current_sentence = next_sentence;
		next_sentence.clear();
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
				next_sentence += current_sentence[j];
			}
		}
	}
}