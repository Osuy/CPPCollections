#pragma once

#include <vector>
#include<algorithm>

// 主模板，因为要存在递归继承，所以先前置声明
template<typename ...F>
struct overload_set;

//// 偏特化1，仅有一个模板参数，递归的终点
//// 继承F1，并using基类的()操作符
//template<typename F1>
//struct overload_set<F1> : public F1
//{
//	overload_set(F1&& f1) : F1(std::forward<F1>(f1)) {}
//	overload_set(const F1& f1) : F1(f1) {}
//
//	// 使用基类F1的()操作符
//	using F1::operator();
//};
//
//// 偏特化2，提取首个模板参数F1
//// 基类为 F1和 overload_set<F...>，递归继承自己，于是会逐个提取出所有模板参数
//// 由于using 了基类的()操作符，于是最终使用了所有基类的()操作符
//template<typename F1, typename ...F>
//struct overload_set<F1, F...> : public F1, public overload_set<F...>
//{
//	overload_set(F1&& f1, F&&...f) : F1(std::forward<F1>(f1)), overload_set<F...>(std::forward<F>(f)...) {}
//	overload_set(const F1& f1, const F& ...f) : F1(f1), overload_set<F...>(f...) {}
//
//	// 使用基类F1的()操作符
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