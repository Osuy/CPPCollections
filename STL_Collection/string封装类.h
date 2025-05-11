#pragma once
#include <string>

class A
{
public:
	std::string s()const {
		return *_s;
	}

	A(std::string& str) :_s(&str) {}
	A(A&& other)
	{
		_s = other._s;
		other._s = nullptr;
	}

	A& operator=(const A& other)
	{
		_s = new std::string(*other._s);
	}

	~A()
	{
		delete _s;
	}

private:
	A(const A& other) 
	{
		_s = new std::string(*other._s);
	}

	std::string* _s;
};