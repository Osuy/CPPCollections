#pragma once

/*
	vc6、bc5、G2.9（现代版本未知）的std.alloctor并没有特殊设计，而是直接调用 ::operator new/delete

	其中G2.9的容器并没有使用allocator，而是使用了alloc。它是经过设计的
	在G4.9里，也同大伙一样用回了标准了allocator，而alloc改名为pool_alloctor

*/

namespace my_allocator
{

template<int inst>
class __malloc_alloc_template
{
};
}