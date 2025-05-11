#pragma once
#include <vector>

void DeleteRepeatElement(const std::vector<int>& v)
{
	int index;
	int arch=0;
	bool repeat;

	for (index = 1; index < v.size(); index++)
	{
		repeat = false;
		for(int i=arch;i>=0;i--)
			if (v[i] == v[index])
			{
				index++;
				repeat = true;
				break;
			}

		if (repeat)
			index++;
		else
			v[++arch] = v[index];
	}
}