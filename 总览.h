#pragma once
/*
	vector
		��������
			random access
	list
		��������
			bidirecrtional
	duque
		��������
			random access
	�����rbtree
		��������
			bidirecrtional
			������ĸ��ڵ�Ҳ�и��ڵ㣬�����ص�nil��nil�ĸ���Ҳ�����ĸ��ڵ�
			nil��������������Ľڵ㣬�Һ����������ҵĽڵ�
			begin()����nil�����ӣ�end()����nil������Ϊ�ܷܺ����ָ����С������߼��ϱ������ǿ����
			�������ʵ��˫��������ʵ�����������˫������

		ģ�������
			Key����ֵ
			Value��ʵֵ
			KeyOfValue����ʵֵ��ȡ����ֵ
			Compare���Ƚϼ�ֵ��С�ĺ�������
			��Value����ȡKey����rbtree��hashtable�϶����õ�����Ϊ������ŵ���Value���������Key����ôΪʲô����Key��Value����Ž�ȥ�أ�
			��ʵ��Ҳ����ô���ġ���map��unoredered_map�Valueʵ������pair<const Key, T>����T����������Ϊ��ʵֵ
			���ƶ��һ�٣�ʵ��������ģ�����������ʵ����const key����map�ļ�ֵ�����Ա��޸ģ�
			���⣬Ҳ�ṩ��һ���Ӳ㣬�����Զ�����δ�Value���ȡKey���Ա�ʵ��һЩ�����ӵĹ���

		������
		1.�к��������ɫ�Ľڵ㣬
		2.���ڵ�Ϊ��ɫ��
		3.���Ӳ���ͬʱΪ��
		4.����ڵ�����������Ҷ�ӽڵ��·���Ϻڽڵ��������
		������Щ���ʣ�������Ժ�ڵ㣬ֻ���ڽڵ㣬��ô�������һ����������������Ҷ���ϸ���ͬһ�㣨����Ϊ�����������캢�ӣ�����֮��һ��4��B����
		���ͬʱ���졢�ڽڵ㣬��ô�������ķ�֧���ں�ں�ں�...���䳤����������̵ķ�֧���ںں�...��������
		���ϣ�������ϸ�ƽ���avl����������Ժڽڵ��ϸ�ƽ�⣬�ֶԺ�ڵ�ʮ�ֿ��ݣ������ڵ��ķ�֧����ƽ��

		��ô��ڵ���ʲô�����أ�
		���ȶ����κ��²���Ľڵ㣬���ȿ�����ɫ����Ϊ������ɫ��ֱ�ӵ��º������ƽ�⣬������ɫ����������˲�ƽ�⣬��ȥ��ɫ����ת
		��Σ���һ���ڽڵ�b�����ĺ캢��r����һ��4��B���Ľڵ㣬�����������b��rb��br��rbr
		��ô�����в���ڵ㣬������������
			1.����b������rb���ұߡ�����br����ߣ�ֱ�Ӳ��뼴��
			2.����rb����ߡ�����br���ұߣ�����һ����ת��Ȼ���ɫ����Ϊrbr�ڵ㼴��
			3.���뵽rbr��rbr���ѣ����׺������ڣ��½ڵ�ֱ�Ӳ��뵽�����¡�Ȼ���游��죬���ϲ��뵽���游���ݹ�ֱ�����ڵ�

		�������3������һ���ݹ�Ĺ��̣�������뵽���ڵ㡣������ڵ���rbr����ô���ڵ�Ҳ����ѣ��������ź�����ĸ߶Ⱦͻ����һ��
		������һ�ź������������0Ϊ�죬1Ϊ�ڣ�
			    1
			0        0
		 1    1    1    1
		0 0  0 0  0 0  0 0
		�����в���һ������ֵ���������ұߣ�����ʹ���ұߵ�����00��Ϊ1�������µ�ֵ��Ϊ����1���ӽڵ�
				1
			0        0
		 1    1    1    1
		0 0  0 0  0 0  1 1
						  0
		Ȼ���游�ڵ��Ϊ0�����ϲ���
				1
			0        0
		 1    1    1    0
		0 0  0 0  0 0  1 1
						  0
		�游���ϲ������Ȼ��һ��rbr�ڵ㣬�����ظ�
				1
			1        1
		 1    1    1    0
		0 0  0 0  0 0  1 1
						  0
		���ڵ�Ϊ�ڣ����Բ����0�����ˣ��������������������Ĳ�����1�����Ҷ����һЩ�����Ľڵ�

		���Է��֣����һ���ڵ��Ǻ�ɫ�ģ���ô��Ҫô��ֱ�Ӳ���ģ�Ҫô�ǵݹ����ġ����������ǡ��������������ƽ�����Ҫԭ��
		��ˣ�������Ժ�ڵ���ݣ������и���Ĳ�ƽ�⣬�����Ż��˲���Ч�ʡ�
		avl����������һ���Ѿ����ɺܶ�Ԫ�ص�avl����ÿ�β��뼸����Ҫ������ת����������˫��
		��������Ĳ��������Ǳ�ɫ�͵��������ٻ�ݹ���롣�����ǵݹ���룬��Ҳ�����ڵ㲻ƽ�⣬�ݹ����ʻ��ڻ�δ������ڵ�ʱ�ͽ���
		��Ҳ���Ǻ��������Ч�ʸ���avl����ԭ��

		����ɾ�������о���
		����++��--
			�����о����ο���������ǰ�����
*/
//������װ���������
//����rbtree��ģ�������Key��Value��KeyOfValue��Compare
//set
template< class Key, class Compare = less<Key>, class Alloc = alloc>
class set
{
	//Key            ����  Key��Value������ֵ����ʵֵ��ֻ���ż�ֵ����
	//identity<Key>  ����  KeyOfValue��identityֱ�ӷ��ز��������������κ��£�����ȡ��ֵkey����ʲôҲ�����ذ�Value����
	//Compare        ����  Compare�����ڱȽ�����key��Ĭ����less<Key>
	typedef rb_tree<Key, Key, identity<Key>, Compare, Alloc> rep_type;
};

//map
template< class Key, class T, class Compare = less<Key>, class Alloc = alloc>
class map
{
	//Key                 ����  Key
	//pair<const Key, T>  ����  Value����ʾmapʵ���ϴ�ŵ���һ��pair����ͬʱ����˼�ֵ��Ԫ��
	//select1s<Value>     ����  KeyOfValue����ȡ��ֵ�Ĳ���������ȡ��pair��first
	//Compare             ����  Compare�����ڱȽ�����key��Ĭ����less<Key>
	typedef pair<const Key, T> Value;
	typedef rb_tree<Key, Value, select1s<Value>, Compare, Alloc> rep_type;
};


/*
	hashtable
		��������
			forward
			hashtable�ǻ���ɢ�еģ���ֵ��û�д�С��ϵ���õ�����ȥ���������������ܵõ���С���������
			��ɢ���ǶԼ�ֵ��ϣ����ȡģ��������Ͱ����53��������Ԫ�ض�С�ڵ���53������������������
			�����ݽṹ���棬��ʹ��Ͱ��+��������Ҳ��ֻ�߱�һ���������������
			���ֻ��forward����

		ģ�������
			Value��ʵֵ
			Key����ֵ
			HashFcn����ϣ������
			ExtractKey����ʵֵ��ȡ����ֵ
			EqualKey���Ƚϼ�ֵ�Ƿ���ȵĺ�������
			��rbtree�Աȣ�����һ��Hash���Ƚϴ�С��Compare����˱Ƚ��Ƿ���ȵ�EqualKey
			ExtractKey�����õ�ͬ��KeyOfValue�����Ǵ�ʵֵ��ȡ����ֵ���������뵽��
			hashtable��rbtreeһ����ֻ���Value�������ṩ����ȡKey�ļ�Ӳ�
			������ͬ����Ϊ��ʵ��unoredered_map

		������
		hashtableʹ��vector��ΪͰ�飬Ͱ���ŵ��ǵ�����ڵ�ָ�룬Ĭ��Ϊ��
		����ɢ�в�����ȡģ�����Ͱ�鳤�������������stl�ṩ��28����������ΪͰ��ĳ��ȣ���һ����ǰһ�����������ң���һ��������53
		hashtable�ĸ���������1������Ԫ����������Ͱ�鳤��ʱ���ͻ�����Ͱ�鳤�ȡ�
		������Ϊ�˱�֤ÿ��Ͱ�ڽ���ֻ��һ��Ԫ�أ���ʵ��O(1)�ķ���
		

*/
//hash_set
template <class Value, class HashFcn = hash<Value>, class EqualKey = equal_to<Value>, class Alloc = alloc>
class hash_set
{
	//Value           ����  Value��Key������ֵ����ʵֵ
	//HashFcn         ����  HashFcn��Ĭ��hash<Value>
	//identity<Value> ����  ExtractKey����ֱ�Ӱ�Value����key
	//EqualKey        ����  EqualKey��Ĭ��equal_to<Value>
	typedef hashtable<Value, Value, HashFcn, identity<Value>, EqualKey, Alloc> ht;
};

//hash_map
template<class Key, class T, class HashFcn = hash<Key>, class EqualKey = equal_to<Key>, class Alloc = alloc>
class hash_map
{
	//pair<const Key, T>   ����  Value
	//Key                  ����  Key
	//HashFcn              ����  HashFcn��Ĭ��hash<Value>
	//select1st<Value>     ����  ExtractKey����pair��first����Key
	//EqualKey             ����  EqualKey��Ĭ��equal_to<Value>
	typedef hashtable<pair<const Key, T>, Key, HashFcn, select1st< pair<const Key, T>> EqualKey, Alloc> ht;
};