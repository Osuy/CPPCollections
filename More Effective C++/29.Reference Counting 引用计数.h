#pragma once
#include <string.h>

/*
	���ʹ��ͬһ�����ݵĶ���ֱ�ӳ������ݣ����������Ѵ��ڵ��Ƿ�
	ֱ������Ҫ�޸�����ʱ�ŷ������ƣ�cow��copy on write ��
	����������ʱ�����ڿ��ܻ�����������������������ݣ�������ֹ�����ͷ�
	ֱ��û���κζ����������ݣ��� ���ü���

	����������Ϊÿһ������׼���ģ�������Ϊ���������ݵĶ���׼���ģ�����������Ƕ���ĳ�Ա���������ݵĳ�Ա

	ʵ�����ü����Ķ���һ��������������ʹ�õĶ����private��protected�ڣ�������struct���Է������

	�������޸�����ǰ����Ҫ�ж��Ƿ���������������������У�����Ҫ����һ�ݸ��Լ�ȥ�޸ģ������������ݵ����ù�ϵ
	���ǣ�����ĳʱ�̣������ռ��һ�����ݣ����ĳ�Ա���������ݵ����ã���operator[]������ȴû���޸�����
	���ǰ����ݵ����ñ����������Ժ��޸ġ���������Ժ�֮ǰ����һ���µĶ�������������
	��ô�Ժ������޸Ļ�Ӱ�쵽��������
	�ڽ���case30������취�ǣ�����һ���ɹ����ǣ�һ��ĳ����const���������ã������Ϊfalse
	���ù�����Ϊfalse������ʱ�����ǿ���
*/

class MyString
{
private:
	struct StringValue
	{
		int ref_count = 0;
		char* data = nullptr;
		bool sharable = true;
		StringValue(const char* init_value)
			: ref_count{ 1 }
		{
			data = new char[strlen(init_value) + 1];
			strcpy(data, init_value);
		}

		~StringValue()
		{
			delete[]data;
		}
	};

	StringValue* value = nullptr;
public:
	// ���ַ����������죬�贴����̬�ڴ档�����Ѵ�����ͬ��string���޷������ǹ�������
	MyString(const char* init_value = "")
		: value{ new StringValue{ init_value } }
	{}

	// �������졣������������ֱ�ӹ�������
	MyString(const MyString& rhs)
	{
		if (rhs.value && rhs.value->sharable)
		{
			++value->ref_count;// ����������string�����������ƣ����ü���ֱ�Ӽ�1
		}
		else
		{
			value = new StringValue{ rhs.value->data };
		}
	}

	// �������ü��������Ϊ0���ͷ�
	~MyString()
	{
		if (0 == --value->ref_count)delete value;
	}

	// 1.֤ͬ��2.���پɵ����ã�3.�����µ�����
	MyString& operator=(const MyString& rhs)
	{
		if (value == rhs.value)return *this;

		if (0 == --value->ref_count)delete value;

		value = rhs.value;
		++value->ref_count;
		return *this;
	}

	// �±������ const ����Ҳ�����޸����ݣ�����char
	char operator[](size_t index)const 
	{
		return value->data[index];
	}

	// �±������ ��const ����ϣ���޸����ݣ�������������������ʱ����ʼ���ƣ�����char&
	char& operator[](size_t index)
	{
		if (value->ref_count > 1)
		{
			value = new StringValue{ value->data };
		}
		value->sharable = false;// �����ݿ��ܱ��޸ģ�����������������
		return value->data[index];
	}
};

/*
	��θ������ü������ܵĴ��룬ʹ����඼ӵ�д˹���
	�������ü��������ڲ���struct���������һ��������struct�̳У����Զ�ӵ�м�������
	�м��������������Է��֣����ӻ����ref_count�Ĳ����󲿷ֻ��Ƿ�����class�������ⲿ�ִ���Ҳ���������������أ�
	class �ڲ����е���һ��struct*������������֧�֣�����ʹ��һ������ָ�룬���乹�졢�����������������ref_count
	��һЩϣ�������õĴ���ԭ��ֲ����Ŀ�����У�������ȡ������һ���µ���RCPtr�������Ѵ��밲��������Ĺ��졢��������ֵ��
	ʹ��Щ�����Զ���ɣ�����ؼ��ٴ�������

	����ref_count�Ĵ��뱻��ȡ������String��ͷ·�ʲôҲû�������Զ����ü�����
	��Ϊ����������destructor��costructor��copy��operator= �ﲻ��Ҫ��д����refcount�Ĵ���
	������������Ҫд destructor��costructor��copy��operator= ��

	RCObject ��ref_count ��ʼʱ��Ϊ0����Ȼ��Ӧ��Ϊ1������Ϊ0���������ĵ�RCPtr::init��дһ��else

	RCPtr ָ��RCObject�����࣬���߳������ݣ���RCPtr�ڸ���ʱֻ��ʹ����copy���죬���ҪRCObject�������Լ��������
*/

class RCObject
{
public:
	RCObject() = default;

	// RCObject������ʱ��������ָ�뱻���ƣ�������ref_count
	// ������������ˣ�˵��������new�����µ�RCObject������ref_count��shareable���ǳ�ʼֵ
	/*RCObject(const RCObject& rhs)
		: ref_count{ 0 }
		, shareable{ true }
	{
	}*/

	// ��һ�ֿ����������������ã���������ͱ��붨�忽�����죬Ȼ��ֻҪ��ֻ�ܶ��常���Ĭ�Ϲ��죬�Ӷ����Ǵﵽ����Ч��
	RCObject(const RCObject& rhs) = delete;

	// ��ֵ���ᱻʹ�ã���Ϊһ����Ĭ�ϸ�ֵ��Ϊ�����ʧȥ�ɵ����ü����������ǡ�����
	RCObject& operator=(const RCObject& rhs) = delete;

	virtual ~RCObject() = 0;

	void addReference()
	{
		++ref_count;
	}

	void RemoveReference()
	{
		if (0 == --ref_count)delete this;// �ڴ�������ϣ�RCObject�������Ȼ����heap�еģ���˿�����ô����
	}

	void MarkUnshareable()
	{
		shareable = false;
	}

	bool IsShareable()const
	{
		return shareable;
	}

	bool IsShared()const { return ref_count > 1; }

private:
	int ref_count = 0;
	bool shareable = true;
};

// ��װ�����������ָ��
template<typename T>
class RCPtr
{
public:
	RCPtr(T* realPtr = nullptr)
		: pointee{ realPtr }
	{
		init();
	}

	RCPtr(const RCPtr& rhs)
		: pointee{ rhs.pointee }
	{
		init();
	}

	~RCPtr()
	{
		if (pointee)
		{
			pointee->RemoveReference();
		}
	}

	RCPtr& operator=(const RCPtr& rhs)
	{
		if (pointee != rhs.pointee)
		{
			if (pointee)
			{
				pointee->RemoveReference();
			}
			pointee = rhs.pointee;
			init();
		}

		return *this;
	}


	T* operator->()const { return pointee; }
	T& operator* ()const { return *pointee; }
private:
	T* pointee = nullptr;
	void init()
	{
		if (!pointee)return;

		if (!pointee->IsShareable())
		{
			pointee = new T{ pointee }; // �˴�����copy���죬���Ҫ��T����߱����������
		}

		pointee->addReference();
	}
};

/*
	���ú��MyString
	������RCPtr��operator[]��Ա�⣬ʲô��û��
	StringValue������Ҫoperator= ����ΪRCPtr���Թ���ɿ�����������ʹ�ñ������Դ��Ŀ���
*/
class MyString2
{
private:
	struct StringValue :public RCObject
	{
		char* data = nullptr;
		StringValue(const char* init_value)
		{
			init(init_value);
		}

		StringValue(const StringValue& rhs) // �����������Ϊ����������rcptr����
		{
			init(rhs.data);
		}

		~StringValue()
		{
			delete[] data;
		}

		void init(const char* init_value)
		{
			data = new char[strlen(init_value) + 1];
			strcpy(data, init_value);
		}

		char  operator[](size_t index) const { return data[index]; }
		char& operator[](size_t index) { MarkUnshareable(); return data[index]; }
	};

	// ������������Stringʱ�����Զ����� RCPtr<StringValue>�Ŀ�������
	// ����ɹ����򣨼��پ����ݵ����ü���������ָ�룬���Ӽ�����
	// ������ɹ����򣨼��پ����ݵ����ü�������������������
	// ��Щ������ȫ����������RCPtr�Ĺ��졢��������������ֵȥִ�У�����Stringʲô��������
	RCPtr<StringValue> value;
public:
	MyString2() = default;
	MyString2(const MyString2&) = default;
	MyString2(const char* str)
		: value{ new StringValue{str} }
	{
	}

	char  operator[](size_t index) const { return value->operator[](index); }
	char& operator[](size_t index) { return value->operator[](index); }
};

/*
	Ϊ����class�������ü�������
	����ֻҪһ��class A�̳���RCObject��������һ��class B�ڲ�����һ��RCPtrָ�����class
	�ڳ�����ȥʹ��B������ʹA�ܹ������ü���

	������Aȥ�̳���RCObject��ζ��Ҫ�޸Ķ��壬���Ҷ��屾�����ܹ����޸ĵ�
	������һ��class���Ե������⣬���޷����޸ģ���������ܹ������ü����أ�

	�����ַ���������ʲô������char[]�����Լ�������һ�����̳��������ࡢҲ���ܱ��޸ĵ�������
	��������Ϊָ�뱻StringValue���еģ�StringValue��ʵ�Լ�Ҳ���Ǹ������������࣬
	��Ҳ�Ǹ���Ӳ㣬ָ��������������

	��ô�����κβ����޸Ķ�����࣬������Ϊ��char[]���Ƶ���Դ���ɡ�ֻ��Ҫ��һ���Ӳ㡣
	������һ����C�̳���RCObject��C�в����޸����ָ���Ա

	��ôMyString��ʵҲ����Ҫ��һ���ڲ���StringValue
	���Ը�Ϊʹ��RCIPtr<char>����Ȼ�ַ����Ĳ�����Щ���⣬������Ҫ�ػ�
*/

template<typename T>
class RCIPtr
{
public:
	RCIPtr(T* real_ptr = nullptr) : counter{ new CountHolder } { counter->pointee = real_ptr; init(); }
	RCIPtr(const RCIPtr& rhs) : counter{ rhs->counter } { init(); }
	~RCIPtr() { counter->RemoveReference(); }
	RCIPtr& operator=(const RCIPtr& rhs)
	{
		if (counter != rhs.counter)
		{
			counter->RemoveReference();
			counter = rhs.counter;
			init();
		}
		return *this;
	}

	// const
	const T* operator->()const { return counter->pointee; }
	const T& operator* ()const { return *counter->pointee; }

	// ��const ��Ҫcow
	T* operator->() { makeCopy(); return counter->pointee; }
	T& operator* () { makeCopy(); return *counter->pointee; }

private:
	struct CountHolder : public RCObject
	{
		~CountHolder() { delete pointee; }
		T* pointee = nullptr;
	};

	CountHolder* counter = nullptr;

	void init()
	{
		// ��ʱ�ǳ�ʼ������δ�������ü���
		// ������ɹ�������һ��
		if (!counter->IsShareable())
		{
			T* oldValue = counter->pointee;
			counter = new CountHolder;
			counter->pointee = new T{ *oldValue };
		}

		// �������ü���
		counter->addReference();
	}

	void makeCopy()
	{
		// ��������ѱ��������ƣ�����ʲô������
		if (counter->IsShared())
		{
			auto oldValue = counter->pointee;
			counter->RemoveReference();
			counter = new CountHolder;
			counter->pointee = new T{ *oldValue };
			counter->addReference();
		}
	}
};

/*
	�ܽ�
		���ö����ࣺ����һ���������ݵ���
		���������ࣺ֧�ֱ�������¼����������Ϳɹ����ǵ�������

		1.���ü����͹�����������������ĳ�Ա���������ö�����ĳ�Ա
		2.����������Ӧ����Ϊ���ö�������ڲ�struct
		3.Ϊ����߸����ԣ���������������ڼ����͹���״̬���߼���ȡ��RCObject����������������̳���RCObject
		4.RCObject�Ŀ�������Ӧ���������ü����͹����ǣ���Ϊ��������������ϣ���������ݣ���������ʱֻ����ָ��
		5.�̳���RCObject���������������֧����������ܴﵽ�����������ݵ�Ч��
		6.Ϊ���������ü����Ĳ��������ö����಻ʹ��ԭ��ָ���Ա����ʹ��RCPtr��Ա�����ڲ���Ԫ��ָ�룬ָ������������
			RCPtr�Թ�ʵ�ֹ��졢��������������ֵ��ȥ������������������ô����͹���״̬���������ö���������������β���
		7.Ϊ�˸������޸ĵ���Ҳ�������ü������ܣ�����������������̳�RCObject���ڲ����и����ָ�롣���������Ӽ�Ӳ�����RCIPtr
*/

namespace my
{
class RCObject
{
	int ref_count = 0;
	bool shareable = true;
public:
	RCObject() = default;
	RCObject(const RCObject&) = delete;
	RCObject& operator=(const RCObject&) = delete;
	virtual ~RCObject() = 0;

	bool is_shared()const { return ref_count > 1; }
	bool is_shareable()const { return shareable; }

	void add_ref() { ++ref_count; }
	void remove_ref() { if (0 == --ref_count)delete this; }
	void unshareable() { shareable = false; }
};

template<typename T>
class RCPtr
{
	T* pointee = nullptr;
public:
	RCPtr(T* ptr = nullptr) : pointee{ ptr } 
	{
		if (!pointee)return;

		if (!pointee->is_shareable())
		{
			pointee = new T{ pointee };
		}

		pointee->add_ref();
	}

	RCPtr(const RCPtr& rhs)
		: pointee{ rhs.pointee }
	{
		if (!pointee)return;
		
		if (!pointee->is_shareable())
		{
			pointee = new T{ pointee };
		}

		pointee->add_ref();
	}


	RCPtr& operator=(const RCPtr& rhs)
	{
		if (pointee == rhs.pointee)return *this;

		if (pointee)
		{
			pointee->remove_ref();
		}

		pointee = new T{ pointee };
		pointee->add_ref();

		return *this;
	}

	~RCPtr() { pointee->remove_ref(); }

	T* operator->()const { return pointee; }
	T& operator* ()const { return *pointee; }
};

class MyString
{
	struct StringValue : public RCObject
	{
		char* data = nullptr;

		StringValue(const char* str = nullptr)
		{
			data = new char[strlen(str) + 1];
			strcpy(data, str);
		}
		StringValue(const StringValue& rhs)
		{
			data = new char[strlen(rhs.data) + 1];
			strcpy(data, rhs.data);
		}

		~StringValue() { delete[] data; }

		char  operator[](size_t index) const { return data[index]; }
		char& operator[](size_t index) { unshareable(); return data[index]; }
	};

	RCPtr<StringValue> value;

public:
	MyString() = default;
	MyString(const char* str)
		: value{ new StringValue{str} }
	{
	}
	char  operator[](size_t index) const { return value->operator[](index); }
	char& operator[](size_t index)		 { return value->operator[](index); }
};
}