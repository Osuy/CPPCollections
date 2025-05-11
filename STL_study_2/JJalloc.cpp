#include<new>
#include<cstddef>
#include<cstdlib>
#include<climits>
#include<iostream>

/*
该配置器所具备的功能：
内存分配allocate：
传进size和类型指针T*，用new分配一块大小为size*sizeof(T)的内存给指针tmp，返回tmp

释放内存deallocate：
传进一个指针buffer，用delete释放buffer所指的空间

构造函数construct：
传进T1指针p和T2的引用value，用value的值和T1的构造函数构造一个变量，T1(value)，并用p指向这个变量

析构函数destroy：
传进一个T类型指针p，调用T的析构~T()。p->~T()

address和max_size：返回地址和最大容量
*/

namespace JJ {

    template<class T>
    inline T* _allocate(ptrdiff_t size, T*) {//基本配置函数，被其他配置器调用
        set_new_handler(0);//new分配内存失败时，调用的函数为0
        T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));//分配一块大小为size*sizeof(T)的内存给指针tmp
        if (tmp == 0) {//tmp所指内存长度为0，说明剩余内存为0，退出配置器
            cerr << "out of memory" << endl;
            exit(1);
        }
        return tmp;//分配成功返回tmp指针
    }

    template <class T>
    inline void _deallocate(T* buffer) {//基本释放函数，调用::operator delete去释放buffer指针
        ::operator delete(buffer);
    }

    template <class T1, class T2>
    inline void _construct(T1* p, const T2& value) {//基本构造函数，用placement new方法
        new(p) T1(value);//T2类型的value初始化一个T1类型的指针，传给p
    }

    template<class T>
    inline void _destroy(T* ptr) {//基本析构函数，调用T的析构函数释放ptr指针
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
        struct rebind {//一个嵌套的类模板，指向另一种未定类型的配置器allocator，命名为other
            typedef allocator<U> other;
        };

        pointer allocate(size_type n, const void* hint = 0) {//配置函数，传进元素个数和空指针hint=0
            //将n从元素个数类型转为指针偏移类型，0从空指针转为T类型指针
            //基本配置函数_allocate获得的参数：偏移量n，T类型的指针
            //返回的是：指向n个元素的内存的指针
            return _allocate((difference_type)n, (pointer)0);
        }

        void deallocate(pointer p, size_type n) {//释放函数，释放指针p所指内存
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