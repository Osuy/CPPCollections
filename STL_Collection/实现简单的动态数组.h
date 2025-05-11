#pragma once
#include <cstdlib>
#include <xmemory>
#include <string>

class DynamicArray
{
public:
	DynamicArray() 
	{
		start = finish = end_of_stroge = nullptr;
	};
	DynamicArray(int n)
	{
		finish = start = (int*)malloc(sizeof(int) * n);
		end_of_stroge = start + n;
	}

	void push(int value)
	{
		if (finish != end_of_stroge)
		{
			*finish = value;
			finish++;
		}
		else
		{
			size_t size = end_of_stroge - start;

			int* p = (int*)malloc(sizeof(int) * size * 2);
			memmove(p, start, size);
			delete start;
			start = p;
			finish = start + size;
			end_of_stroge = start + 2 * size;
		}
	}

	int pop()
	{
		if (start >= finish)return -1;
		
		int num = *--finish;

		size_t size = (end_of_stroge - start) / 2;

		if (size > finish - start)
		{
			int* p = (int*)malloc(sizeof(int) * size);
			memmove(p, start, size);
			delete start;
			start = p;
			finish = start + size;
			end_of_stroge = start + 2 * size;
		}

		return num;
	}

	int get(int idx)
	{
		if (nullptr == start || idx > finish - start)return -1;
		
		return  *(start + idx);
	}

private:
	int* start;
	int* finish;
	int* end_of_stroge;
};