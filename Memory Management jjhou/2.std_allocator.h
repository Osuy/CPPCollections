#pragma once

/*
	vc6��bc5��G2.9���ִ��汾δ֪����std.alloctor��û��������ƣ�����ֱ�ӵ��� ::operator new/delete

	����G2.9��������û��ʹ��allocator������ʹ����alloc�����Ǿ�����Ƶ�
	��G4.9�Ҳͬ���һ���û��˱�׼��allocator����alloc����Ϊpool_alloctor

*/

namespace my_allocator
{

template<int inst>
class __malloc_alloc_template
{
};
}