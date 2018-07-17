/*================================================================
*   Copyright (C) 2018 xuboxuan. All rights reserved.
*   
*   文件名称：syncqueue.h
*   创 建 者： xuboxuan
*   创建日期：2018年07月17日
*   描    述：
*
================================================================*/


#ifndef _SYNCQUEUE_H
#define _SYNCQUEUE_H

#include <condition_variable>
#include <mutex>
#include <iostream>
#include <thread>
#include <queue>
template<typename T>
class SyncQueue{
public:
	explicit SyncQueue(): m_need_stop(false){};
	
	void Put(T x){Add(x);}

	
	void Pop(T& t){
		std::unique_lock<std::mutex> locker(m_mutex);
		m_notempty.wait(locker, [this](){return m_need_stop || NotEmpty();});
		
		if(m_need_stop ) return;
		t = m_queue.front();
		m_queue.pop();
	}

	void Stop(){
		{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_need_stop = true;
		}

		m_notempty.notify_all();
	}
	

	bool Empty() {
		std::lock_guard<std::mutex> locker(m_mutex);
		return m_queue.empty();
	}

	size_t Size(){
		std::lock_guard<std::mutex> locker(m_mutex);
		return m_queue.size();
	}
private:
	bool NotEmpty(){
		bool empty = m_queue.empty();
		if(empty) std::cout << "empty, waiting, thred id" << std::this_thread::get_id() << std::endl;
		return !empty;
	}


	template<typename F>
	void Add(F&& x){
		std::unique_lock<std::mutex> locker(m_mutex);
		if(m_need_stop) return;
		m_queue.push(std::forward<F>(x));
		m_notempty.notify_one();
	}
private:
	std::mutex m_mutex;
	std::condition_variable m_notempty;
	std::queue<T> m_queue;
	bool m_need_stop;
};
#endif //SYNCQUEUE_H
