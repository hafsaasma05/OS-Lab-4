#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <chrono>

class SimpleThreadPool {
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mtx;
    std::condition_variable cv;
    bool stop = false;
public:
    SimpleThreadPool(size_t threads) {
        for(size_t i = 0; i < threads; ++i)
            workers.emplace_back([this] {
                while(true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queue_mtx);
                        this->cv.wait(lock, [this]{ return this->stop || !this->tasks.empty(); });
                        if(this->stop && this->tasks.empty()) return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    task();
                }
            });
    }
    template<class F>
    void enqueue(F&& f) {
        {
            std::unique_lock<std::mutex> lock(queue_mtx);
            tasks.emplace(std::forward<F>(f));
        }
        cv.notify_one();
    }
    ~SimpleThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queue_mtx);
            stop = true;
        }
        cv.notify_all();
        for(std::thread &worker: workers) worker.join();
    }
};

int main() {
    SimpleThreadPool pool(2);
    pool.enqueue([]{ std::cout << "Task 1 executed by thread pool.\n"; });
    pool.enqueue([]{ std::cout << "Task 2 executed by thread pool.\n"; });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return 0;
}