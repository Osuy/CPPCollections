#include<new>
#include<cstddef>
#include<cstdlib>
#include<climits>
#include<iostream>

/*
�����������߱��Ĺ��ܣ�
�ڴ����allocate��
����size������ָ��T*����new����һ���СΪsize*sizeof(T)���ڴ��ָ��tmp������tmp

�ͷ��ڴ�deallocate��
����һ��ָ��buffer����delete�ͷ�buffer��ָ�Ŀռ�

���캯��construct��
����T1ָ��p��T2������value����value��ֵ��T1�Ĺ��캯������һ��������T1(value)������pָ���������

��������destroy��
����һ��T����ָ��p������T������~T()��p->~T()

address��max_size�����ص�ַ���������
*/

namespace JJ {

    template<class T>
    inline T* _allocate(ptrdiff_t size, T*) {//�������ú���������������������
        set_new_handler(0);//new�����ڴ�ʧ��ʱ�����õĺ���Ϊ0
        T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));//����һ���СΪsize*sizeof(T)���ڴ��ָ��tmp
        if (tmp == 0) {//tmp��ָ�ڴ泤��Ϊ0��˵��ʣ���ڴ�Ϊ0���˳�������
            cerr << "out of memory" << endl;
            exit(1);
        }
        return tmp;//����ɹ�����tmpָ��
    }

    template <class T>
    inline void _deallocate(T* buffer) {//�����ͷź���������::operator deleteȥ�ͷ�bufferָ��
        ::operator delete(buffer);
    }

    template <class T1, class T2>
    inline void _construct(T1* p, const T2& value) {//�������캯������placement new����
        new(p) T1(value);//T2���͵�value��ʼ��һ��T1���͵�ָ�룬����p
    }

    template<class T>
    inline void _destroy(T* ptr) {//������������������T�����������ͷ�ptrָ��
        ptr->~T();
    }

    template <class T>
    class allocator {

    public:
        typedef T value_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        template<U>
        struct rebind {//һ��Ƕ�׵���ģ�壬ָ����һ��δ�����͵�������allocator������Ϊother
            typedef allocator<U> other;
        };

        pointer allocate(size_type n, const void* hint = 0) {//���ú���������Ԫ�ظ����Ϳ�ָ��hint=0
            //��n��Ԫ�ظ�������תΪָ��ƫ�����ͣ�0�ӿ�ָ��תΪT����ָ��
            //�������ú���_allocate��õĲ�����ƫ����n��T���͵�ָ��
            //���ص��ǣ�ָ��n��Ԫ�ص��ڴ��ָ��
            return _allocate((difference_type)n, (pointer)0);
        }

        void deallocate(pointer p, size_type n) {//�ͷź������ͷ�ָ��p��ָ�ڴ�
            _deallocate(p);
        }

        void construct(pointer p, const_reference value) {
            _construct(p, value);
        }

        void destroy(pointer p) {
            _destroy(p);
        }

        pointer address(reference x) {
            return (pointer)&x;
        }

        const_pointer const_address(const_reference x) {
            return (const_pointer)&x;
        }

        size_type max_size()const {
            return size_type(UINT_MAX / sizeof(T));
        }
    };
}