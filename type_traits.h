#pragma once

namespace yw
{
	struct false_type
	{
		char padding[8];
	};

	typedef char true_type;

	template<class T>
	struct is_class_impl
	{
		template<class U>
		static yw::true_type is_class_tester(void(U::*)(void));

		template<class U>
		static yw::false_type is_class_tester(...);

		static const bool value = sizeof(is_class_tester<T>(0)) == sizeof(true_type);
	};

	template<class T>
	struct is_class
	{
		static const bool value = yw::is_class_impl<T>::value;
	};

	template<class T>
	struct is_array
	{
		static const bool value = false;
	};

	template<class T, size_t N>
	struct is_array<T[N]>
	{
		static const bool value = true;
	};

	template<int>
	struct eight_class
	{
		static const bool value = false;
	};

	template<>
	struct eight_class<8>
	{
		static const bool value = true;
	};

	//模板
	template<class T>
	struct classA{};
	//偏特化
	template<class T>
	struct classA<T*> {};
	//全特化
	template<>
	struct classA<int> {};
}