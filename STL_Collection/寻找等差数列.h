#pragma once

/*
	将多个等长的等差数列打乱后混在一起，
	求等差数列的最大可能的长度
*/

int hash[60];
int n, x, ans, maxnum;
int work(int now)
{
	int first, second, delta, i;
	int ok;

	while (now <= maxnum && !hash[now])
		++now;

	if (now > maxnum)return 1;

	first = now;
	for (second = first; second <= maxnum; second++)
	{
		if (hash[second])
		{
			delta = second - first;
			if (first + delta * (ans - 1))
				break;
			else
			{
				ok = 1;
				for (i = 0; i < ans; ++i)
					ok = hash[first] >= ans && (hash[first + delta * i]);
			}

			if (ok)
			{
				for (i = 0; i < ans; ++i)hash[first + delta * i]--;
				if (work(first))
					return 1;
				for (i = 0; i < ans; ++i)hash[first + delta * i]++;
			}
		}
	}
}