#pragma once
#include <thread>
#include <string>
#include <functional>
#include <vector>
#include <algorithm>
#include <iostream>

/*
	2.1�����ܿ�
	ÿ��C++����������һ���̣߳���main�̣߳���C++����ʱϵͳ����
	�����߳�����������Ϊ��ڡ���һ���Ǻ������κο�ִ�ж��󶼿�����Ϊ���
	main����renturnʱ��main�߳̽����������˳�
	��ں���returnʱ�������߳̽���
	std::thread �ܿ��߳�

	�߳��� std::thread ����ʱ��ʼִ��

	������ִ�ж�����Ϊ std::thread �Ĳ���ʱ�������Ϊ�������������Խ�����{}������
*/

struct func_obj
{
	void operator()() {}
};

void test1()
{
	// ������Ϊ���������� ������t1������ֵstd::thread��������һ������ָ�루����ֵfunc_obj���޲Σ���������������
	std::thread t1(func_obj()); 
	
	// ����һ��Բ����ʹ���ܽ���Ϊ����
	std::thread t2((func_obj())); 
	
	// ��ʽ��ʼ�������ᱻ����Ϊ��������
	std::thread t3{ func_obj{} };

	//t1.join(); // t1 ���Ƕ���
	t2.join();
	t3.join();
}

/*
	std::thread��������߳��Ѿ���ʼִ�У������ڶ�������֮ǰ������Ҫ������ϻ��Ƿ���
	���磬һ���ֲ��� std::thread ����ִ��һ�����ӵĺ������������뿪������ʱ���̻߳�δ����
	���Ǵ�ʱ���ֲ���std::thread����Ҫ���������������������߳�û�л�ϻ���룬�ͻ����std::terminate��ֹ��������
		��ϣ�����join���ȴ��߳̽���
		���룺����detach�����ȴ�������ִ�б��̣߳�����std::thread�����Ѿ�����

	join������û�ã�
	ȷʵ��������»�ʹ��detach�����̺߳����߳�ͬʱ����
	������̵߳ȴ����߳�ִ�����ټ�������ô�ȴ��Ĺ����б��̱߳�������
	����������߳��Ƿ����˶���̷߳�������ͬʱ���У���������������ɺ�ż���
	��ô��Ӧ��ʹ��join

	
	���һ���߳��Է���ķ�ʽ���У���������ʹ���������̵߳ľֲ����������ã����ǲ��ܱ�֤�����õĶ������������Ƿ����
	���ʹ���˸����ã��ͻᷢ��δ������Ϊ
*/

void test2()
{
	int local_integer = 0;

	std::thread t1{ [&local_integer]() {} }; 
	t1.detach();
	// t1���룬test2���أ��ֲ�����local_integer�ͷţ�����t1�߳���������local_integer
	// ʹ��local_integer�ᷢ��δ������Ϊ
	// ���t1 join���򲻻��������������Ϊtest2�ض���t1�߳̽������return
}

/*
	�߳��쳣
	����std::thread����������ǰ�����ϻ����
	��ôһ���ֶ���ϻ����Ĵ�������д�ںδ���Ҫ����һ�����⣺
		����֮ǰ��������쳣����ô�Ͳ��������ϣ�����std::thread���������
	��������쳣�����ܱ��׳������������ǻ���Ϊstd::thread����������������std::terminate����ֹ����
	����취�ǰ��̶߳���Ļ�Ͽ�������Դ������Դ�����ฺ��
	��Ӧ��ʹ�÷��룬�÷����쳣����̻߳�����ִ�У���Ϊ���߳̿��ܻ����ñ��̵߳ı����������û��
	���̰߳�ȫ����
*/

struct thread_guard
{
	std::thread& t;
	thread_guard(std::thread& T) :t{ T } {}

	// ��ֹ���ƣ���Ϊ���ƿ���������λ�ϣ������������Ѿ��ж���joinable�����Ը���Ҳû�£�
	thread_guard(const thread_guard&) = delete;
	thread_guard& operator=(const thread_guard&) = delete;
	~thread_guard()
	{
		if (t.joinable())
		{
			t.join();
		}
	}
};

void test3()
{
	std::thread t{ []() {} };
	thread_guard tg{ t };

	// to other thing
	// ��ʱ���㷢���쳣��tg��������t�̣߳�������ֹ����
}

/*
	�����std::thread�������߳����޹��������ɻ�ϡ��̵߳Ĺ���Ȩ�Ϳ�������ת�Ƹ�C++����ʱ��
	��Щ�̱߳������ػ��̣߳�ͨ���᳤ʱ���ں�̨����

	ͬ��������joinable���ܵ���detach
	*/
	
	
/*
	2.2���̺߳�������
	������ȷһ���£����̺߳������ݲ��� �� ��std::thread�Ĺ��캯�����ݲ��� ��������ͬ����

	std::thread �Ĺ��캯��������������������һ�ѳ���һ����������ں�����������ں���
	��Ϊstd::thread�Ĺ�����һ����κ���ģ��
	����Ȼ�Ǻ���ģ�壬��ζ����ֻ��һ��һʮ�ذ�ʵ���Ƶ�

	���ǣ���ں����Ĳ���������string������std::thread���촫��Ŀ�����һ��const char*
	����Ϊ�Σ�
*/


void test4()
{
	// ������ں�����const char*����ں�����һ��string���͵Ĳ���
	// ����ڶ������ǲ�����ʽת��Ϊstring�ı����������ʧ��
	std::thread t1{ [](std::string) {}, "void func" }; 
	t1.join();

	//std::thread t2{ [](std::string) {}, 1.f }; // error��δ�ҵ�ƥ������غ���
	//t2.join();
}

/*
	������Ϊ std::thread ���캯���´����������б������ٽ����Ǵ�����ں���
	��������ģ���Ƶ�ʱ����鴫�����ͺ���ں��������Ƿ�һһ��Ӧ
	���Ա���ʧ�ܵı����ǣ�invokeδ�ҵ�ƥ������غ����������Ǻ���ģ���Ƶ�ʧ��
	
	����const char* ���Թ���string�����Ե�����ں���ʱ���ṹ�������string����

	���ǣ��������������char*������ָ����ַ����п�������ں���ִ��ǰ�Ѿ�������������
	��ô���� ��char* ����string�ͻᷢ��δ������Ϊ

	���ִ���������ǣ�1.ʹ�õ�ָ��ֲ�������ָ�룻2.�̷߳���
*/

void test5()
{
	char buffer[1024];
	sprintf_s(buffer, "%i", 1024);

	// �߳�����ʱ��test4�����Ѿ���������ʱbufferָ������������string�ͻ���δ������Ϊ
	std::thread t1{ [](std::string) {}, "void func" };
	t1.detach();
}

/*
	�����Ȼ���� ���� std::thread ����Ĳ������ͺ���ں����Ĳ���������ȫƥ��

	std::thread �Ĺ��쿽����������һ���ص㣬����ȫ��������ʱ������������ں���ʱ��ʵ�ε����ͻ�����ֵ
	���ֵ����û��Ӱ�죬���Ƕ������ô��ݣ��ᷢ���������δ�ҵ�ƥ������غ���
*/

void test6()
{
	std::string str{ __func__ };
	std::thread t1{ [](std::string&) {}, str }; // error��δ�ҵ�ƥ������غ���
	t1.detach();

	// ���������bind���ƣ���֮ͬ������bind�����ı����ᱣ���ڿ�ִ�ж����ڲ�������ʵ�ββ�����ֵ
	// bind�󶨵ĺ������Դ����ã������б�����󣬵��ﲻ��Ԥ��
	// ��Ϊ���õ��ǿ�ִ�ж����ڲ��ı������������ⲿ��Ŀ�����
	auto op1 = std::bind([](std::string&) {}, str); // ����str��bind���������ջ´������ִ�ж�����
	op1();

	// ����취��ʹ��std::ref
	std::thread t2{ [](std::string&) {}, std::ref(str) };
	t2.detach();

	auto op2 = std::bind([](std::string&) {}, std::ref(str)); // ����str��bind���������ջ´������ִ�ж�����
	op2();
}

/*
	�Գ�Ա����Ϊ��ں�������ڶ����������Ƕ���ָ�룬��ں����Ĳ����ӵ�������ʼ����
*/

void test7()
{
	struct Aop
	{
		void func(int) {}
	};

	Aop obj;
	std::thread t1{ Aop::func, &obj, 1 }; // �ڶ������Ƕ���ָ�룬����������ʼ���Ǵ���
}

/*
	std::thread �Ĺ���������ôһ����Ӹ����ǣ�
		1.�������в���
		2.����ֵ����ʽ������ں���
	��������Ҫôʹ��std::ref����std::thread���죬��Կ��ܵ��������ڽ����ķ���
	Ҫô��ں����Ĳ���ʹ��ֵ���ݣ���Կ������Ӷ���Ķ��⿪��

	���ڣ�C++11�������ƶ����壬������ͨ��ת������Ȩ�����⿽��
	�ƶ�����Ҫ���ֵ���ݣ���Ϊֵ���ݲŻṹ���βΣ���ֵ/��ֵ���ô��ݲ��ṹ���βΣ�
	����ʵ������ֵ���βι���ʱ��ƥ�䵽�ƶ����캯�����Ӷ������ƶ�
*/

void test8()
{
	std::string str{ __func__ };
	std::thread t1{ [](std::string) {}, std::move(str) };// ��ں�����Ϊֵ����
	t1.detach();

	/*
		��std::thread ���� std::move(str)�������Ǹ���ֵ�����Կ�����ֱ��Ų���䶯̬�ڴ�
		��������str�Ѿ������Ϳա��������ٱ�ʹ�ã���str�������ѱ��ƶ���std::thread������
		��Ȼ�󴫵ݸ���ں�������Ȼ�β���ֵ���ݣ�����ͨ����ʱ�����죬�����ƶ�����ʱ�����Ķ�̬�ڴ�ֱ��Ų�����β���
		����������û�з��������Ŀ�����ֻ�Ƕ��������ƶ�
	*/
}

/*
	2.3 std::thread ����Ҳ֧���ƶ������Թ��� std::thread ���󲢷���֮ǰ�� thread_guard ����ʹ�� std::thread& �ķ�ʽ
	���һ����������һ��std::thread�����ظ����÷������𣬻��߲�����std::thread���󣬶����ɲ�������
	������������ת��std::thread�Ĺ���Ȩ���������std::thread&������ʵ��
	
	ʵ���ϴ��ֱ��ʹ��std::thread���ɣ���Ҫת�Ƶĵط���ʹ��std::move��ƥ���ƶ����죬ʵ��ת��

	���һ��std::thread�Ѿ�������һ���̣߳��Ͳ���������ֵ���߳�
*/

void test9()
{
	auto func1 = [] {};
	auto func2 = [] {};

	std::thread t1{ func1 };
	std::thread t2 = std::move(t1); // ��t1�ƽ���t2

	t1 = std::thread{ func2 }; // t1�ٴι���һ���µ��߳�

	std::thread t3; // �ն���t3���������κ��߳�
	t3 = std::move(t2); // t2�ƽ���t3
	t1 = std::move(t3);// t3�ƽ���t1������t1�Ѿ�������һ���̡߳�������ܣ��ͻ�ʹ���е��̱߳���������Զ�޷���Ϻͷ��룬���Ի����terminate��ֹ����
}

/*
	thread_guard���޸�
	��Ϊstd::thread֧���ƶ����壬�����������ٸ������õ���ʽ������
	���û����г����������ڵ�����
*/

class scoped_thread
{
	std::thread t;
public:
	explicit scoped_thread(std::thread t_) // �β��Ǵ�ֵ������贫����ֵ���Զ������ƶ�
		: t{ std::move(t_) }// ֱ�ӽ��β��ƶ�����Ա����t
	{
		if (!t.joinable())
		{
			throw std::logic_error{ "No thread" };
		}
	}

	~scoped_thread()
	{
		t.join();
	}

	scoped_thread(const scoped_thread&) = delete;
	scoped_thread& operator =(const scoped_thread&) = delete;
};

/*
	C++20Ҳ��һ�������Զ���ϵ��࣬joining_thread����scoped_threadʮ������
*/

class joining_thread
{
	std::thread t;
public:
	// Ĭ�Ϲ��캯�����������߳�
	joining_thread() noexcept = default;

	// ���ģ�幹�죬���ܿ�ִ�ж���Ͳ�����ֱ�ӹ��� std::thread 
	template<typename Callable, typename... Args>
	explicit joining_thread(Callable&& func, Args&&...args)
		: t{ std::forward<Callable>(func), std::forward<Args>(args)... }
	{}

	// �����Ѵ��ڵ�std::thread ��ʱ����
	explicit joining_thread(std::thread t_) noexcept
		: t{ std::move(t_) }
	{}

	// �ƶ�����
	joining_thread(joining_thread&& other) noexcept
		: t{ std::move(other.t) }
	{
	}

	// �ƶ���ֵ
	joining_thread& operator =(joining_thread&& other) noexcept 
	{
		// ����Լ�������߳̿��Ի�ϣ��Ȼ��
		if (joinable())
			join();

		t = std::move(other.t);
		return *this;
	}

	// �ƶ���ֵ2
	joining_thread& operator =(std::thread&& other) noexcept 
	{
		// ����Լ�������߳̿��Ի�ϣ��Ȼ��
		if (joinable())
			join();

		t = std::move(other);
		return *this;
	}

	~joining_thread()noexcept
	{
		if (joinable())
			join();
	}

	void swap(joining_thread& other)noexcept
	{
		t.swap(other.t);
	}

	std::thread::id get_id()const noexcept
	{
		return t.get_id();
	}

	bool joinable()const noexcept
	{
		return t.joinable();
	}

	void join()
	{
		t.join();
	}

	void detach()
	{
		t.detach();
	}

	std::thread& as_thread()noexcept
	{
		return t;
	}

	const std::thread& as_thread()const noexcept
	{
		return t;
	}
};

/*
	����thread_guard/scoped_thread/joining_thread��RAII��
	�߳�Ҳ���Խ�������������������ʱ����������Ԫ�ء�������Ҫ����������������ǰ������Ԫ��join

	һ�����͵ļ������񣬲��Ҹ�����֮��û�й��������Զ��Լ��㣬�Ϳ���ʹ�ö��߳������м���
	����������ڱ�ʹ��ǰ������ȷ�������̶߳��Ѿ�������ϣ����Ӧ�ö������̵߳���join���
	Ȼ�����ʹ�ü�����
	���磺��͡���͵�ÿ���ֶ��Ƕ����ģ���͵Ľ��Ҳ����ȴ�ÿ�����ֽ�������ܵõ�
*/

void test10()
{
	auto func = [](int i) {};
	std::vector<std::thread> threads;

	for (int i = 0; i < 20; ++i)
	{
		threads.emplace_back(func, i);
	}

	for (auto& entry : threads)
	{
		entry.join();
	}
}

/*
	2.4�����̵߳�����
	C++��׼���ṩ��һ������hardware_cocurrency()�����ڻ�ȡ��ǰ����ʹ�õ��߳�����
	����һ�������ϵõ��ļ�������ʹ�����������߳���ǡ����
	�����Ϊ��

	�����߳�ִ�е���ں�����û�з���ֵ�ģ�Ҳ�޷��õ�����ֵ��������Ҫ�������ã�������std::ref
*/

template<typename Iterator , typename T>
struct accumulate_block
{
	void operator()(Iterator first, Iterator last, T& result)
	{
		result = std::accumulate(first, last, result);
	}
};

template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
	unsigned long const length = std::distance(first, last);
	if (!length)
		return init;

	//���߳�����
	unsigned long const min_per_thread = 25;
	unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;
	auto hardware_threads = std::hardware_concurrency();
	auto num_threads = std::min(hardware_threads ? 2 : hardware_threads, max_threads);
	
	// �����С
	auto block_size = length / num_threads;
	
	std::vector<T> results(num_threads);
	std::vector<std::thread> threads(num_threads - 1);// ���߳�Ҳ��һ���߳�

	Iterator block_start = first;
	for (unsigned long i = 0; i < (num_threads - 1); ++i)
	{
		Iterator block_end = block_start;
		std::advance(block_end, block_size);
		threads[i] = std::thread{ accumulate_block<Iterator, T>{}, block_start, block_end, std::ref(results[i]) };
		block_start = block_end;
	}

	// ���̸߳��������һ��block�������ڻ��֮ǰ���������ܺ������̲߳��м��㣩
	accumulate_block<Iterator, T>{}.(block_start, last, std::ref(results[num_threads - 1]));

	// �ȴ������̻߳��
	for (auto& entry : threads)
	{
		entry.join();
	}

	return std::accumulate(results.begin(), results.end(), init);
}

/*
	2.5 �߳�id
	std::thread::id ����������������ѧ�����hash
	std::this_thread::get_id
*/

void test11()
{
	std::cout << std::this_thread::get_id();
}