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
		//��ʾ���ι��캯��
		explicit ThreadPool(int num) : _thread_num(num), _is_running(false)
		{}

		//���ÿ����͸�ֵ����
		ThreadPool(const ThreadPool&) = delete;
		ThreadPool& operator=(const ThreadPool& other) = delete;

		void start()
		{
			_is_running = true;

			for (int i = 0; i < _thread_num; ++i)
			{
				//����num���̣߳�ÿ���߳�ִ��work
				_threads.emplace_back(thread(&ThreadPool::work, this));
			}
		}

		void stop()
		{
			{
				//�������޸�״̬��Ȼ���������߳�
				//����_is_runningΪfalse�����ѵ��̰߳���ͷ��ִ������ֹͣ��
				std::unique_lock<mutex> lock(_mtx);
				_is_running = false;
				_cond.notify_all();
			}

			//���������̣߳��������ִ�У�����תִ��
			for (thread& t : _threads)
			{
				if (t.joinable())
					t.join();
			}
		}

		//�������
		void appendTask(const Task& task)
		{
			if (_is_running)
			{
				//���������У�������task��ӣ�Ȼ����һ���߳�
				unique_lock<mutex> lock(_mtx);
				_tasks.push(task);

				//notify_one�Ƿ�ỽ�Ѳ�ִ��δjoin���̣߳�
				_cond.notify_one();
			}
		}

	private:

		//ÿһ��work������һ���߳�ȥִ��
		//��ˣ���work�����_cond.wait(lock)���ͻ��������߳�
		//�߳�ֻ��stop��appendTask�﻽��
		void work()
		{
			printf("begin work thread :%d \n", this_thread::get_id());

			//��д������
			//һ��ѭ�������ж�is_running���˷�����
			//������Ҫ��task�п�

			//�ĳ�while(true)
			//û������return
			//�����������п�ֱ��ִ��
			while (_is_running)
			{
				Task task;

				//work����������л�ȡ�����������Ҫ������work���⣬���������
				//���û�����񣬴��߳�����
				//�ȵ�������ʱ����appendTask������
				{
					unique_lock<mutex> lock(_mtx);
					if (!_tasks.empty())
					{
						//������в��գ�����һ������
						task = _tasks.front();
						_tasks.pop();
					}
					else if (_is_running && _tasks.empty())
					{
						//������пգ��������߳�
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