#pragma once
#include <mutex>
#include <list>

/*
	һ���̵߳ȴ���һ���̼߳���������ż���
	�Ⲣ��join��ϡ�
	��һ���̼߳�����������������Ҫ����������Ȼ�������У��������ϵؼ���
	�����߳�Ҳ�������У�ֻ����ĳЩ����ͣ�٣��ȴ���һ�̵߳Ľ��
	�����Ĺ��̳����������൱�������߳��ں�������

	�������̺߳�����������ơ�����������Ҫ�������ݣ����Կ���������Ϊȫ�ֱ�����
	������һ��������������Ƿ����
	��Ȼ���ڶ��̷߳��ʣ�������Ҫ���Ᵽ��
*/

std::mutex mtx;
int some_data;
bool usable = false;
void set_some_data(int sd)
{
	std::unique_lock lk{ mtx };
	usable = true;
	some_data = sd;
}

void use_some_data()
{
	while (!usable);
	std::unique_lock lk{ mtx };
	usable = false;
	some_data;// ʹ��some_data
}

/*
	�߳�1�������ϵط��� usable
	���߳�2���ú���some_data���߳�1��������ס����ʹ��some_data

	���������ȱ�����ڣ��߳�1��usable�Ĳ�ͣ�ؽ���������ķ��ʣ�����ʹ���������½����ﲻ�����ö��߳�������ܵ�Ŀ��
	��������sleep���߳�1�ķ���usable����ôƵ������������sleep��ã���ԶԶ����cpuʱ������
	�ǳ��򡰱����������ܵ��¡�
*/

/*
	4.4����ͬ���򻯴���

	1.����ʽ���
	���ǹܵ����ľ޴��������Ĵ������������Ӵ����Ա���Ϊ�˸�������
	Ȼ�󽻸��ܵ��еĶ������ȥ��ɡ�ÿ���������������һ������������
	ȱ�����ڣ�1.���ܴ�����Щ�������������ݾͿ��Լ���Ķ������ݣ���˿������ö��߳�
	2.�л������൱���л��������ģ����״̬���뱣�������������⣬����ȫ�ֱ��������Ա

	���ö��߳�ͬ�������������񽻸����̣߳�������future��������ֻ����������е�ĳ��ʱ��ȡfuture��ֵ����
	�����߼���ͬһƬ�������У��������ݿ����ɲ������ݶ�������ȫ�ֱ������������͵���
	���ֲ�����ȫ�ֱ����ĵ����������� ������ȫ���������ķ���ֵ��������Ķ��κ��ⲿ״̬�ֳ� ��pure����
	������������ʹ�õĺ�����ֻҪ����Ĳ�����ͬ���õ��Ľ��Ҳ��Ȼ��ͬ���������Ӱ��

	������Ϊ��
*/

template<typename T>
std::list<T> sequential_quick_sort(std::list<T> input)
{
	if (input.empty())
	{
		return input;
	}

	std::list<T> result;

	result.splice(result.begin(), input, input.begin()); // ��input����Ԫ���ƶ��� result
	const T& pivot = *result.begin();
	auto divide_point = std::partition(input.begin(), input.end(), [&](const T& t) {return t < pivot;}); // ��С��pivot��Ԫ����������ǰ�棬���غ�벿�ֵĵ�һ��Ԫ�صĵ�����

	std::list<T> lower_part;
	lower_part.splice(lower_part.end(), input, input.begin(), divide_point);// ��ǰ�벿���ƶ���lp

	// �ݹ�������� 
	// ��lp�ƶ���sequential_quick_sort�Ĳ�����������ֵ����
	// ͬʱ���ܷ���ֵ����һ���µ�������sequential_quick_sort�ķ���ֵҲ��ֵ���ݵģ��Զ��ƶ�
	// ��������û�з����κο���
	auto new_lower(sequential_quick_sort(std::move(lower_part));

	// ����input��ʣ�µĺ�벿����ͬ���Ĳ���
	auto new_higher(sequential_quick_sort(std::move(input));

	// ��ǰ�벿���ƶ���resultǰ�棬��벿���ƶ���result���档��pivot��ǰ�Ѱ�����result�
	result.splice(result.begin(), new_lower);
	result.splice(result.end(), new_higher);

	return result;
}

/*
	ע�⵽��������ǰ�벿��ʱ����������벿�֣������ǿ��������̴߳���ǰ�벿�֣�ͬʱ��һ���̴߳����벿��
	���߳��������ʱ��future.getȡ�ú�벿�ֵĽ������

*/