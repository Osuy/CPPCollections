#pragma once
#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <thread>
#include <mutex>
#include <atomic>
//#include <condition_variable>
#include <functional>
#include <vector>
#include <queue>

using std::thread;
using std::mutex;
using std::atomic;
using std::condition_variable;
using std::function;
using std::vector;
using std::queue;

namespace osuy
{
	class ThreadPool
	{
	public:
		using Task = function<void()>;

	private:
		atomic<bool> _is_running;
		mutex _mtx;
		condition_variable _cv;
		int _thread_num;
		vector<thread> _threads;
		queue<Task> _tasks;

	public:
		//显示单参构造函数
		explicit ThreadPool(int num) : _thread_num(num), _is_running(false)
		{}

		//禁用拷贝和赋值构造
		ThreadPool(const ThreadPool&) = delete;
		ThreadPool& operator=(const ThreadPool& other) = delete;

		void start()
		{
			_is_running = true;

			for (int i = 0; i < _thread_num; ++i)
			{
				//创建num个线程，每个线程执行work
				_threads.emplace_back(thread(&ThreadPool::work, this));
			}
		}

		void stop()
		{
			{
				//上锁，修改状态，然后唤醒所有线程
				//由于_is_running为false，唤醒的线程把手头的执行完后就停止了
				std::unique_lock<mutex> lock(_mtx);
				_is_running = false;
				_cond.notify_all();
			}

			//遍历所有线程，如果可以执行，则跳转执行
			for (thread& t : _threads)
			{
				if (t.joinable())
					t.join();
			}
		}

		//添加任务
		void appendTask(const Task& task)
		{
			if (_is_running)
			{
				//若还在运行，上锁，task入队，然后唤醒一个线程
				unique_lock<mutex> lock(_mtx);
				_tasks.push(task);

				//notify_one是否会唤醒并执行未join的线程？
				_cond.notify_one();
			}
		}

	private:

		//每一个work都交给一个线程去执行
		//因此，在work里调用_cond.wait(lock)，就会阻塞该线程
		//线程只在stop和appendTask里唤醒
		void work()
		{
			printf("begin work thread :%d \n", this_thread::get_id());

			//此写法不好
			//一是循环总是判断is_running，浪费性能
			//二是需要对task判空

			//改成while(true)
			//没有任务return
			//有任务无需判空直接执行
			while (_is_running)
			{
				Task task;

				//work从任务队列中获取任务，这个过程要与其他work互斥，因此需上锁
				//如果没有任务，此线程阻塞
				//等到有任务时，由appendTask来唤醒
				{
					unique_lock<mutex> lock(_mtx);
					if (!_tasks.empty())
					{
						//任务队列不空，出队一个任务
						task = _tasks.front();
						_tasks.pop();
					}
					else if (_is_running && _tasks.empty())
					{
						//任务队列空，阻塞本线程
						_cond.wait(lock);
					}
				}

				if (task)
				{
					task();
				}
			}

			printf("end work thread :%d \n", this_thread::get_id());
		}



	};
}
#endif