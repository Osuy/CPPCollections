#pragma once

/*
	hash_set�����в������ǵ�תhashtable�Ĳ������˴���������˵��
	ע�⣺
		hash_setû��set���Զ�������
		hash_set��ģ������б��ﲻ����Key��ExtracKey��
		hash_set��insert���õ���hanshtable��insert_unique
*/
template<class Value,class HashFcn = hash<value>,class EqualKey = equal_to<Value>, class Alloc=alloc>
class hash_set
{
	typedef hashtable<Value, Value, HashFcn, identity<Value>, EqualKey, Alloc> ht;
	//���ϣ���Value��Ϊhashtable��Key����identity<Value>��Ϊhashtable��ExtractKey
	//�Ӷ�ʵ����set��ֵ����ʵֵ���ص�
};


/*
	hash_map�����в���Ҳ��תhashtable
	ע�⣺
		hash_mapû��map���Զ�������
		hash_map�洢pair<const Key, T>
		hash_map��insert���õ���hanshtable��insert_unique
		
*/
template <class Key,class T ,class HashFcn=hash<Key>,class EqualKey = equal_to<Key>,class Alloc=alloc>
class hash_map
{
private:
	typedef hashtable<pair<const Key, T>, Key, HashFcn, select1st<pair<const Key, T>>, EqualKey, Alloc> ht;
	//����hashtableֻ�洢Value��key��ͨ��ExtractKey(Value)����õ�
	//��hash_map��pair<const Key,T>��ΪValue,select1st<pair<const Key,T>>��ΪExtractKey
	//�Ӷ�ͬʱ�洢��Value��Key������pair.first����ȡkey
	//ʵ��map��ֵ��ʵֵ������ص�
};

/*
	hash_multiset��hash_set��Ψһ�������insert���õ���hanshtable��insert_equal
	hash_map��hash����multimap������ͬ��
*/