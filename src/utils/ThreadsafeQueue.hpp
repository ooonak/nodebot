#pragma once

#include <mutex>
#include <queue>
#include <memory>
#include <condition_variable>

/**
 * Implementation from Anthony Williams "C++ Concurrency in Action, Second Edition 2019" p. 179.
 */

template<typename T>
class ThreadsafeQueue {
private:
    mutable std::mutex mtx_;
    std::queue<T> dataQueue_;
    std::condition_variable dataCond_;

public:
    ThreadsafeQueue() {};

    void push(T value)
    {
        std::lock_guard<std::mutex> lock(mtx_);
        dataQueue_.push(std::move(value));
        dataCond_.notify_one();
    }

    void waitAndPop(T& value)
    {
        std::unique_lock<std::mutex> lock(mtx_);
        dataCond_.wait(lock, [this]{ return !dataQueue_.empty(); });
        value = std::move(dataQueue_.front());
        dataQueue_.pop();
    }

    bool tryPop(T& value)
    {
        std::lock_guard<std::mutex> lock(mtx_);
        if (dataQueue_.empty())
        {
            return false;
        }
        value = std::move(dataQueue_.front());
        dataQueue_.pop();

        return true;
    }

    std::shared_ptr<T> tryPop()
    {
        std::lock_guard<std::mutex> lock(mtx_);
        if (dataQueue_.empty())
        {
            return std::shared_ptr<T>();
        }
        
        std::shared_ptr<T> res(std::make_shared<T>(std::move(dataQueue_.front())));
        dataQueue_.pop();

        return res;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(mtx_);
        return dataQueue_.empty();
    }
};
