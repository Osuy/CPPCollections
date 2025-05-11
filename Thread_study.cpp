#include <iostream>
#include <vector>
#include<ctime>
#include <chrono>
#include <mutex>
#include <thread>
#include "ThreadPool.h"
using namespace std;

void foo()
{
    cout << "foo execute" << endl;
}

void foo2(int arg)
{
    cout << "foo2 execute" << endl 
        << "arg is: " << arg << endl;
}

int main()
{

    //ThreadPool pool(8);
    //vector< future<int> > results;
    //mutex mtx;
    //time_t start, end;

    //int num = 1000000;
    //int temp = 0;
    //
    //start = clock();
    //for (int i = 0; i < 4; ++i) {
    //    results.emplace_back(
    //        pool.enqueue([i, num, &temp] {
    //        //std::unique_lock<std::mutex> lock(mtx);
    //        //std::cout << "hello " << i << std::endl;
    //        //std::this_thread::sleep_for(std::chrono::seconds(1));
    //        //std::cout << "world " << i << std::endl;
    //        for (int i = 0; i < num; ++i)
    //        {
    //            //cout << i * i - i % 3 + i / 4 << ' ';
    //            temp = i * i - i % 3 + i / 4;
    //        }
    //        return 1;
    //    })
    //    );
    //}

    //for (auto&& result : results)
    //{
    //    //std::unique_lock<std::mutex> lock(mtx);
    //    //cout << result.get()<<' ';
    //    temp = result.get();
    //}
    //end = clock();
    //cout << (double)(end - start) / CLOCKS_PER_SEC << endl;
    //

    thread first(foo);
    thread second(foo2, 1);

    first.detach();
    second.detach();


    
    return 0;
}