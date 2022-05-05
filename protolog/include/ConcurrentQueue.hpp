#ifndef CONCURRENT_QUEUE_HPP
#define CONCURRENT_QUEUE_HPP

#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>
#include <atomic>

namespace Protolog
{
	template<typename T, typename BasicQueue = std::queue<T>>
	class ConcurrentQueue
	{
	private:
	    mutable std::mutex mut;
	    BasicQueue data_container;
	    std::condition_variable cond_var;
	public:
	    ConcurrentQueue()
	    {

		}
	    ConcurrentQueue(ConcurrentQueue const& other)
	    {
	        std::lock_guard<std::mutex> lock{other.mut};
	        data_container=other.data_container;
	    }

	    void push(T value)
	    {
	        std::lock_guard<std::mutex> lock{mut};
	        data_container.push(value);
	        cond_var.notify_one();
	    }

	    void wait_and_pop(T& value)
	    {
	        std::unique_lock<std::mutex> lock{mut};
	        cond_var.wait(lock,[this]
				{
					return !data_container.empty();
				});
	        value = data_container.front();
	        data_container.pop();
	    }

	    std::shared_ptr<T> wait_and_pop()
	    {
	        std::unique_lock<std::mutex> lock{mut};
	        cond_var.wait(lock,[this]
			{
				return !data_container.empty();
			});
	        std::shared_ptr<T> res(std::make_shared<T>(data_container.front()));
	        data_container.pop();
	        return res;
	    }

	    bool try_pop(T& value)
	    {
	        std::lock_guard<std::mutex> lock{mut};
	        if(data_container.empty())
	            return false;
	        value = data_container.front();
	        data_container.pop();
	        return true;
	    }

	    std::shared_ptr<T> try_pop()
	    {
	        std::lock_guard<std::mutex> lock{mut};
	        if(data_container.empty())
	            return std::shared_ptr<T>();
	        std::shared_ptr<T> res(std::make_shared<T>(data_container.front()));
	        data_container.pop();
	        return res;
	    }

	    bool empty() const
	    {
	        std::lock_guard<std::mutex> lock{mut};
	        return data_container.empty();
	    }

		size_t size() const
		{
			std::lock_guard<std::mutex> lock{mut};
			return data_container.size();
		}
	};
}

#endif