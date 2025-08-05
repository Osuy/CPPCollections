#pragma once

/*
	ģ���������ϰ����class��typename����������ʷԭ�򣬻��ַ������١���һЩ�µĿ��Ƽ�ʹ��typename
	����������struct����class������ʡ�Լ̳кͳ�Ա���public
	�޺�׺��ʵ����
	��׺_t������ģ�壬һ����ʽΪ����::type������ʡ�ԡ�::type���������ı�
	��׺_v������ģ�壬��������::value��Ҳ������ֱ�ӱ���
		һЩʵ��������ǻ���integral_constants<XXX_v<T>>ʵ�ֵ�

	C++�ж�������arr[N]�Ͳ���������arr[]����˹��������traits�����������汾��ƫ�ػ�
	�����bounded_array��ֻ�ж���ƫ�ػ��������unbounded_array��ֻ�в�������ƫ�ػ�

	ƫ�ػ�ģ������б��ڵ�ƥ��ʧ�ܲ���SFINAE
	����Ͷ�������Ƶ�ʧ�ܻ��������Ĵ���
	void_t����������ƫ�ػ������б����Ƶ��õ�
*/
// false �����������κ�ģ���������false
template<typename> constexpr bool _Always_false = false;

// ���γ���
template<typename T, T val>
struct integral_constant
{
	static constexpr T value = val;
	using value_type = T;
	using type = integral_constant;

	constexpr operator value_type()const noexcept { return  value; }
	constexpr value_type operator()()const noexcept { return  value; }
};

// bool�����������ͳ����ı���ģ��
template<bool val>using bool_constant = integral_constant<bool, val>;

using true_type = bool_constant<true>;
using false_type = bool_constant<false>;

/*
	conjunction���Ӳ��������ҵ���һ��valueΪfalse������
	����һ��bool�Ϳɱ�ģ�������Ҫ��ÿ��������value��Ա
	��ģ�壺boolƥ��false��ȡ�������ĵ�һ����Ϊtype
	ƫ�ػ���boolƥ��true����ʾ���ܲ������ĵ�һ�͵ڶ������ݹ鴫�ݵڶ�::value��ʣ�������
		
	���������ڵĳ�Ա������value��Աʱ���������
*/
template<bool first_value, typename First, typename...>
struct _Conjunction {
	using type = First;
};

template<typename First, typename Next, typename... Rest>
struct _Conjunction<true, First, Next, Rest...> {
	using type = _Conjunction<Next::value, Next, Rest...>::type;
};

template<typename... T> struct conjunction : true_type {};
template<typename First, typename...Rest> 
struct conjunction<First, Rest...> : _Conjunction<First::value, First, Rest...>::type {};

template<typename... T>
constexpr bool conjunction_v = conjunction<T...>::value;

// ȡ�� �̳���bool_constant����ֵΪT::valueȡ��
template<typename T> struct negation : bool_constant<!static_cast<bool>(T::value)> {};

template <typename _Trait> constexpr bool negation_v = negation<_Trait>::value;

// ��ʶ�࣬ͨ��������Ϊ���࣬��ʡ��using type = XXX;�����ٴ�����
template<typename T> struct identity { using type = T; };

template<typename T> using identity_t = identity<T>::type;

/*
	remove_cv �Ƴ������е�const��volatile
	��ģ��ƥ��ԭʼ����
	ƫ�ػ�ƥ��const��volatile��const volatile ���ֲ�ͬ������

*/
template<typename T> struct remove_cv					 : identity<T> {};
template<typename T> struct remove_cv<const T>			 : identity<T> {};
template<typename T> struct remove_cv<volatile T>		 : identity<T> {};
template<typename T> struct remove_cv<const volatile T> : identity<T> {};

template<typename T> using remove_cv_t = typename remove_cv<T>::type;

// ���������Ƿ���ͬ
template<typename, typename>  constexpr bool is_same_v		= false;
template<typename T>		constexpr bool is_same_v<T, T>  = true;

template<typename T1, typename T2>
struct is_same : bool_constant<is_same_v<T1, T2>> {};

// �Ƿ�Ϊvoid����
template <typename _Ty>
constexpr bool is_void_v = is_same_v<remove_cv_t<_Ty>, void>;

// void_t ������ֱ��ʹ�õļ�ֵ��������SFINAE���൱��һ����ģ������Ƶ�����
template<typename... T>using void_t = void;

// add_const \ add_volatile \ add_cv ����const\volatile\const volatile����
template<typename T>struct add_const	 :identity<const T> {};
template<typename T>struct add_volatile :identity<volatile T> {};
template<typename T>struct add_cv		 :identity<const volatile T> {};

template<typename T>using add_const_t	  = add_const<T>::type;
template<typename T>using add_volatile_t = add_volatile<T>::type;
template<typename T>using add_cv_t		  = add_cv<T>::type;

// is_reference �Ƿ�Ϊ����
// template<typename>constexpr bool is_reference_v = false;
template<typename T>constexpr bool is_reference_v<T&> = true;
template<typename T>constexpr bool is_reference_v<T&&> = true;
template<typename T>struct is_reference :bool_constant<is_reference_v<T>> {};

template<typename>constexpr bool is_lvalue_reference_v = false;
template<typename T>constexpr bool is_lvalue_reference_v<T&> = true;
template<typename T>struct is_lvalue_reference :bool_constant<is_lvalue_reference_v<T>> {};

template<typename>constexpr bool is_rvalue_reference_v = false;
template<typename T>constexpr bool is_rvalue_reference_v<T&&> = true;
template<typename T>struct is_rvalue_reference :bool_constant<is_rvalue_reference_v<T>> {};

/*
	add_reference Ϊ���������������
	��ģ��ƥ�䲻���������ͣ�void�ͺ���ָ�룩�����еڶ���ģ�������������Ĭ��void��Ŀ������ƫ�ػ�����SFINAE
	ƫ�ػ��汾ƥ����������ͣ�����ʹ��void_t���������Ƶ����Ƶ��ɹ���ʾ�������ǿ���������
	���ƫ�ػ���Ҫ��ģ���еڶ���ģ�����

	����һ�������۵����ľ仰��
		��ֵ����ֵ
		��ֵ����ֵ
		��ֵ��������ֵ
		��ֵ��������ֵ����Ҫ��

	�ô������ʽ����
		T& & = T&
		T&& & = T&
		T& && = T&
		T&& && = T&&

	���Լ�ʹ�����ô��� add_reference<...> Ҳ������ȷ�Ƶ���ʵ�ʵ���ֵ����ֵ��
	��T = T&���� void_t<T& &> �Ƶ��ɹ���ƥ��ɹ�
	���� lvalue = T&  & = T&; rvalue = T& && = T&;
*/
template<typename T, typename = void>
struct add_reference
{
	using lvalue = T;
	using rvalue = T;
};

template<typename T>
struct add_reference<T, void_t<T&>> // ʹ��void_t����SFINAE
{
	using lvalue = T&;
	using rvalue = T&&;
};

template<typename T> struct add_lvalue_reference { using type = add_reference<T>::lvalue; };
template<typename T> struct add_rvalue_reference { using type = add_reference<T>::rvalue; };
template<typename T> using add_lvalue_reference_t = add_lvalue_reference<T>::type;
template<typename T> using add_rvalue_reference_t = add_rvalue_reference<T>::type;

/*
	declval() һ�������Ƶ���������ĳ�Ǿ�̬��Ա�����Ĺ���
	��û��ʵ�֣�����ʵ���ڲ�Ϊ����false��static assert
	��˲��ܱ����ã�ֻ�ܳ�����decltype(...)�ڲ�
	���ķ���ֵ��T&& ��˿�����.�����á��Ǿ�̬��Ա���������Ƶ����䷵��ֵ������
*/
template<typename T>
add_rvalue_reference_t<T> declval() noexcept;

// is_pointer

// remove_pointer �Ƴ�ָ������
template<typename T>struct remove_pointer					 :identity<T> {};
template<typename T>struct remove_pointer<T*>				 :identity<T> {};
template<typename T>struct remove_pointer<T* const>			 :identity<T> {};
template<typename T>struct remove_pointer<T* volatile>		 :identity<T> {};
template<typename T>struct remove_pointer<T* const volatile> :identity<T> {};

template <typename T> using remove_pointer_t = typename remove_pointer<T>::type;

// add_point���ָ�����Σ� T�����Ƿ�ԭʼ���ͣ�������Ҫremove_reference
template<typename T, typename = void>
struct add_pointer :identity<T> {};
template<typename T>
struct add_pointer<T, void_t <remove_reference_t<T>*>> 
{
	using type = remove_reference_t<T>*;
};

template <typename _Ty> using add_pointer_t = typename add_pointer<T>::type;

// is_array �Ƿ������� 
template<typename>			   constexpr bool is_array_v	   = false;
template<typename T>		   constexpr bool is_array_v<T[]>  = true;
template<typename T, size_t N>constexpr bool is_array_v<T[N]> = true;
template<typename T>struct is_array :bool_constant<is_array_v<T>> {};

template<typename>			   constexpr bool is_bounded_array_v = false;
template<typename T, size_t N>constexpr bool is_bounded_array_v<T[N]> = true;
template<typename T>struct is_bounded_array :bool_constant<is_bounded_array_v<T>> {};

template<typename>			   constexpr bool is_unbounded_array_v = false;
template<typename T>		   constexpr bool is_unbounded_array_v<T[]> = true;
template<typename T>struct is_unbounded_array :bool_constant<is_unbounded_array_v<T>> {};

/*
	remove_extent �Ƴ�����ά�ȣ��Ƶ�������Ԫ�ص�����
	remove_all_extent �Ƴ����������ά�ȣ��Ƶ�������Ԫ������
*/
template<typename T>			struct remove_extent :identity<T> {};
template<typename T>			struct remove_extent<T[]> :identity<T> {};
template<typename T, size_t N> struct remove_extent<T[N]> :identity<T> {};

template <typename _Ty> using remove_extent_t = typename remove_extent<_Ty>::type;

template<typename T>			struct remove_all_extent :identity<T> {};
template<typename T>			struct remove_all_extent<T[]> :remove_all_extent<T> {};
template<typename T, size_t N> struct remove_all_extent<T[N]> :remove_all_extent<T> {};

template <typename T> using remove_all_extent_t = typename remove_all_extent<T>::type;

void test()
{
}