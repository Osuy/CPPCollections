#pragma once

#include <vector>
#include<algorithm>

// ��ģ�壬��ΪҪ���ڵݹ�̳У�������ǰ������
template<typename ...F>
struct overload_set;

//// ƫ�ػ�1������һ��ģ��������ݹ���յ�
//// �̳�F1����using�����()������
//template<typename F1>
//struct overload_set<F1> : public F1
//{
//	overload_set(F1&& f1) : F1(std::forward<F1>(f1)) {}
//	overload_set(const F1& f1) : F1(f1) {}
//
//	// ʹ�û���F1��()������
//	using F1::operator();
//};
//
//// ƫ�ػ�2����ȡ�׸�ģ�����F1
//// ����Ϊ F1�� overload_set<F...>���ݹ�̳��Լ������ǻ������ȡ������ģ�����
//// ����using �˻����()����������������ʹ�������л����()������
//template<typename F1, typename ...F>
//struct overload_set<F1, F...> : public F1, public overload_set<F...>
//{
//	overload_set(F1&& f1, F&&...f) : F1(std::forward<F1>(f1)), overload_set<F...>(std::forward<F>(f)...) {}
//	overload_set(const F1& f1, const F& ...f) : F1(f1), overload_set<F...>(f...) {}
//
//	// ʹ�û���F1��()������
//	using F1::operator();
//};

template<typename ... F>
struct overload_set : public F ... {
	overload_set(F &&... f) : F(std::forward<F>(f)) ... {}

	using F::operator();
};

template<typename ... F>
auto overload(F &&... f) {
	return overload_set<F ...>(std::forward<F>(f) ...);
}

int test_combinate_lambda()
{
	auto helper = overload([]() {}, [](int) {}, [](float) {});

	//helper();
	//helper(1);
	//helper(1.f);
	return 1;
}