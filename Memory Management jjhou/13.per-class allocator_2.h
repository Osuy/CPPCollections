#pragma once

class AirPlane
{
private:
	struct AirPlaneRep
	{
		unsigned long miles = 0;
		char type = 0;
	};

	union
	{
		AirPlaneRep rep;
		AirPlane* next;
	};
	enum { BLOCK_SIZE = 512 };
	static AirPlane* head_of_freelist;

public:
	unsigned long get_miles()const { return rep.miles; }
	char get_type()const { return rep.type; }

	void set(unsigned long m, char t)
	{
		rep.miles = m;
		rep.type = t;
	}

	static void* operator new(size_t);
	static void operator delete(void*, size_t);
};