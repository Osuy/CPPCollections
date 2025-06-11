#include "12.per-class allocator_1.h"

Screen* Screen::freeStore = nullptr;

void* Screen::operator new(size_t size)
{
	Screen* p = nullptr;
	if (!freeStore)
	{
		size_t chunk = screenChunk * size;
		freeStore = p = reinterpret_cast<Screen*>(new char[chunk]);

		for (;p != &freeStore[screenChunk - 1];++p)
		{
			p->next = p + 1;
		}

		p->next = nullptr;
		p = freeStore;
		freeStore = freeStore->next;
		return p;
	}
}

void Screen::operator delete(void*data, size_t)
{
	(static_cast<Screen*>(data))->next = freeStore;
	freeStore = static_cast<Screen*>(data);
}
