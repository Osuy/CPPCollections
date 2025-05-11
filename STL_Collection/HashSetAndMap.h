#pragma once

/*
	hash_set的所有操作都是调转hashtable的操作，此处不做过多说明
	注意：
		hash_set没有set的自动排序功能
		hash_set的模板参数列表里不包含Key和ExtracKey。
		hash_set的insert调用的是hanshtable的insert_unique
*/
template<class Value,class HashFcn = hash<value>,class EqualKey = equal_to<Value>, class Alloc=alloc>
class hash_set
{
	typedef hashtable<Value, Value, HashFcn, identity<Value>, EqualKey, Alloc> ht;
	//以上，用Value作为hashtable的Key，用identity<Value>作为hashtable的ExtractKey
	//从而实现了set键值就是实值的特点
};


/*
	hash_map的所有操作也调转hashtable
	注意：
		hash_map没有map的自动排序功能
		hash_map存储pair<const Key, T>
		hash_map的insert调用的是hanshtable的insert_unique
		
*/
template <class Key,class T ,class HashFcn=hash<Key>,class EqualKey = equal_to<Key>,class Alloc=alloc>
class hash_map
{
private:
	typedef hashtable<pair<const Key, T>, Key, HashFcn, select1st<pair<const Key, T>>, EqualKey, Alloc> ht;
	//由于hashtable只存储Value，key是通过ExtractKey(Value)来获得的
	//而hash_map让pair<const Key,T>作为Value,select1st<pair<const Key,T>>作为ExtractKey
	//从而同时存储了Value和Key，并用pair.first来获取key
	//实现map键值和实值分离的特点
};

/*
	hash_multiset与hash_set的唯一区别就是insert调用的是hanshtable的insert_equal
	hash_map与hash——multimap的区别同理
*/