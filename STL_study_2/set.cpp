/*
	set�ĵײ���rbtreeʵ�ֵ�
	set������������Ԫ�ػ��Զ����򡣣�set��ʵֵ���Ǽ�ֵ��
	set�ĵ��������ܶ�Ԫ���޸ģ��������Բ����ɾ���������const_iterator
*/
#include <functional>
using namespace std;

template <class Key,class Compare=less<Key>,Alloc=alloc>
class set {
public:
	typedef Key key_type;
	typedef Key value_type;
	typedef Compare key_compare;
	typedef Compare value_compare;

private:
	typedef rb_tree<Key_type, value_type, identity<value_type>, key_compare, Alloc> rep_type;

	rep_type t;

public:
	typedef typename rep_type::const_pointer pointer;
	typedef typename rep_type::const_pointer const_pointer;
	typedef typename rep_type::const_reference reference;
	typedef typename rep_type::const_reference const_reference;
	typedef typename rep_type::const_iterator iterator;
	typedef typename rep_type::const_iterator const_iterator;
	typedef typename rep_type::const_reverse_iterator reverse_iterator;
	typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
	typedef typename rep_type::size_type size_type;
	typedef typename rep_type::difference_type difference_type;

	//���캯������Ĭ�ϻ�ָ���ıȽϺ�������t��Ȼ��ʲô������
	set() : t(Compare()) {}
	explicit set(const Compare& comp) :t(comp) {}

	//�������캯��
	set(const set<Key, Compare, Alloc>& x) :t(x.t) {}

	//�����Ĺ��캯�����÷����ĵ���������һ�������t��ֵ��ʹ��rbtree��insert_unique
	template<class InputIterator>
	set(InputIterator first, InputIterator last) : t(Compare()) { t.insert_unique(first, last); }

	//�����丳ֵ��ͬʱָ���ȽϺ���
	template<class InputIterator>
	set(InputIterator first, InputIterator last, const Compare& comp) : t(comp) { t.insert_unique(first, last); }

	//��������ء�����==��<�Ƿ�������Ԫ.set�����ģ���������ʡ�ԣ������Ŀ�ģ���������ʡ��
	set<Key, Compare, Alloc>& operator=(const set<Key, Compare, Alloc>& x) { t = x.operator=; return *this; }
	friend bool operator==<>(const set&, const set&);
	friend bool operator<<> (const set&, const set&);

	//��ȡ������������rbtree
	key_compare key_comp()const { return t.key_comp(); }
	value_compare value_comp()const { return t.key_comp(); }//ʵֵ����ֵ
	iterator begin()const { return t.begin(); }
	iterator end()const { return t.end(); }
	reverse_iterator rbegin()const { return t.rbegin(); }
	reverse_iterator rend()const { return t.rend(); }
	bool empty()const { return t.empty(); }
	size_type size()const { return t.size(); }
	size_type max_size()const { return t.max_size(); }
	void swap(set<Key, Compare, Alloc>& x) { t.swap(x.t); }

	//�޸ĺ����������ɾ��
	typedef pair<iterator, bool> pair_iterator_bool;
	pair<iterator, bool> insert(const value_type& x) {
		pair<typename rep_type::iterator, bool>p = t.insert_unique(x);//����󷵻ص���һ������
		return pair<iterator, bool>(p.first, p.second);//�������rep_type::iteratorת��Ϊiterator
	}

	iterator insert(iterator position, const value_type& x) {
		typedef typename rep_type::iterator rep_iterator;
		return t.insert_unique((rep_iterator&)position, x);
	}

	template<class InputIterator>
	void insert(InputIterator first, InputIterator last) {
		t.insert_unique(first, last);
	}

	void erase(iterator position) {
		typedef typename rep_type::iterator rep_iterator;
		t.erase((rep_iterator&)position);
	}
	
	size_type erase(const key_type& x) {
		return t.erase(x);
	}

	void erase(iterator first, iterator last) {
		typedef typename rep_type::iterator rep_iterator;
		t.erase((rep_iterator&)first, (rep_iterator&)last);
	}

	void clear() { t.clear(); }

	//set�ṩ�ĺ���
	iterator find(const key_type& x)const { return t.find(x); }

	size_type count(const key_type& x)const { return t.count(x); }

	iterator lower_bound(const key_type& x)const { return t.lower_bound(x); }

	iterator upper_bound(const key_type& x)const { return t.upper_bound(x); }

	pair<iterator, iterator> equal_range(const key_type& x)const { return t.equal_range(x); }

};

//������Ԫ����
template<class Key,class Compare ,class Alloc>
inline bool operator==(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
	return x.t == y.t;//rbtree��==����
}

template<class Key, class Compare, class Alloc>
inline bool operator==(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
	return x.t < y.t;
}

