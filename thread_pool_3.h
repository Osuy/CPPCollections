#pragma once

#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>
#include <thread>
#include <atomic>
using namespace std;

class thread_pool_3
{
public:
	using Task = function<void()>;
private:
	struct data
	{
		mutex _mtx;
		condition_variable _cv;
		atomic<bool>  _is_running = false;
		queue <Task> _tasks;
	};

	shared_ptr<data> _data;

public:
	explicit thread_pool_3(size_t thread_count)
		:_data(make_shared<data>())
	{
		for (size_t i = 0; i < thread_count; ++i)
		{
			thread([data = _data] {}
			).detach();
		}
	}

private:
	void work()
	{
		unique_lock<mutex> guard(_data->_mtx);

		while(true)
		{
			if (!_data->_tasks.empty())
			{
				Task task = move(_data->_tasks.front());
				_data->_tasks.pop();

				guard.unlock();
				task();
				guard.lock();
			}
			else if (!_data->_is_running)
			{
				break;
			}
			else
			{
				_data->_cv.wait(guard);
			}
		}

	}
};