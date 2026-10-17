#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>

std::mutex mtx;
std::condition_variable cv;
std::queue<int> task_queue;
bool finished = false;

void producer() {
    for (int i = 1; i <= 3; ++i) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            task_queue.push(i);
            std::cout << "Produced item: " << i << std::endl;
        }
        cv.notify_one();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    {
        std::lock_guard<std::mutex> lock(mtx);
        finished = true;
    }
    cv.notify_all();
}

void consumer() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return !task_queue.empty() || finished; });
        
        if (!task_queue.empty()) {
            int item = task_queue.front();
            task_queue.pop();
            lock.unlock();
            std::cout << "Consumed item: " << item << std::endl;
        } else if (finished) {
            break;
        }
    }
}

int main() {
    std::thread p(producer);
    std::thread c(consumer);
    p.join();
    c.join();
    return 0;
}