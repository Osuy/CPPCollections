/*
	������º�������Ҫ����
	�ӷ����������˷���������ȡ����ȡ��
	���STL�㷨ʹ�ã����Ǹ�����Ա�Լ���
*/

template<class T>
struct plus :public binary_function<T, T, T> {
	T operator()(const T& x, const T&y)const {
		return x + y;//�Ը������ͣ���Ҫ����+�����,��ͬ
	}
};

template<class T>
struct minus :public binary_function<T, T, T> {
	T operator()(const T& x, const T& y)const {
		return x - y;
	}
};

template<class T>
struct multiplies :public binary_function<T, T, T> {
	T operator()(const T& x, const T& y)const {
		return x * y;
	}
};

template<class T>
struct divides :public binary_function<T, T, T> {
	T operator()(const T& x, const T& y)const {
		return x / y;
	}
};

template<class T>
struct modulus :public binary_function<T, T, T> {
	T operator()(const T& x, const T& y)const {
		return x % y;
	}
};

template<class T>
struct multiplies :public unary_function<T, T> {
	T operator()(const T& x)const {
		return -x;
	}
};

void text() {
	//���ֵ��÷�ʽ

	//���庯�������ö���������
	plus<int> p;
	int result=p(1, 3);

	//�������������
	//�������ţ���һ���������������������󣬵ڶ������ŵ��õ�()����
	//���е��ֺţ����������ͷ�
	plus<int>()(1, 3);
}