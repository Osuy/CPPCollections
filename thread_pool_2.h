#pragma once
#ifndef _THREAD_POLL_2_H_
#define _THREAD_POLL_2_H_
#endif 

#include <vector>
#include <queue>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

#define MAX_THREAD_NUM 256

using namespace std;

class thread_pool_2
{
public:
	using Task = function<void()>;

private:
	vector<thread> _threads;
	queue<Task> _tasks;
	mutex _mtx;
	condition_variable _cv;
	atomic<bool> _is_running;
	atomic<int> _free_thread_num;

public:

	thread_pool_2(unsigned short size = 4) :_is_running(false)
	{
		_free_thread_num = size < 1 ? 1 : size;

		for (size = 0; size < _free_thread_num; ++size)
		{
			_threads.emplace_back(&thread_pool_2::work, this);
		}
	}

	~thread_pool_2()
	{
		_is_running.store(true);
		_cv.notify_all();
		for (thread& thread : _threads)
		{
			if (thread.joinable())
				thread.join();
		}
	}

	//start函数
	//stop函数
	//析构函数内调用stop，确保线程池关闭

	template<class F,class...Args>
	auto commit(F&&f,Args&&... args) -> future<decltype(f(args...))>
	{
		if (!_is_running.load())
			throw runtime_error("commit on thread pool is stopped.");

		using RetType = decltype(f(args...));

		auto task = make_shared<packaged_task<RetType()>>(
			bind(forward<F>(f), forward<Args>(args)...)
			);

		future<RetType> future = task->get_future();

		//添加任务到队列
		{
			lock_guard<mutex> lock(_mtx);

			_tasks.emplace([task]() {(*task)(); })
		}

		_cv.notify_one();

		return future;
	}

	int free_thread_num() { return _free_thread_num; }

private:
	void work()
	{
		while (_is_running)
		{
			Task task;
			{
				unique_lock<mutex> lock(_mtx);
				//当不在运行，且没有任务时，阻塞该线程
				_cv.wait(lock, [this]
					{
						return !_is_running || !_tasks.empty();
					}
				);

				if (_is_running && _tasks.empty())
					return;

				task = std::move(_tasks.front());
				_tasks.pop();
			}

			{
				lock_guard<mutex> lock(_mtx);
				--_free_thread_num;
			}
			
			task();

			{
				lock_guard<mutex> lock(_mtx);
				++_free_thread_num;
			}
		}
	}

};