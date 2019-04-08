#ifndef _SAFESTACK_H_
#define _SAFESTACK_H_

#include <vector>
#include <mutex>

template<class T> class SafeStack
{
	// This class is written keeping in mind that only pop() operation will be needed
	// push() can be used, however not preferred as it is slower due to vector resizes

	public:
	
	std::vector<T> sstack;
	std::mutex sslock;
	int size;
	int progress = 0;

	SafeStack(const std::vector<T>& _sstack) : sstack(_sstack) {}
	SafeStack(int _size) : size(_size)
	{
		sstack.reserve(_size);
	}

	bool pop(T &item);
	void push(const T& item);
};

template<class T> bool SafeStack<T>::pop(T &item)
{
        std::lock_guard<std::mutex> lock(sslock);

        if(sstack.empty())
        {
                return false;
        }

        item = sstack.back();
        sstack.pop_back();

	progress++;
        return true;
}

template<class T> void SafeStack<T>::push(const T& item)
{
        std::lock_guard<std::mutex> lock(sslock);

        sstack.push_back(item);
}

#endif
