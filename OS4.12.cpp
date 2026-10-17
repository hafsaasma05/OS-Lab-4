#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <condition_variable>

// Dev-C++ compatible custom Counting Semaphore
class SimpleSemaphore {
    std::mutex mtx;
    std::condition_variable cv;
    int count;
public:
    explicit SimpleSemaphore(int count_ = 0) : count(count_) {}

    void acquire() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return count > 0; });
        --count;
    }

    void release() {
        std::unique_lock<std::mutex> lock(mtx);
        ++count;
        cv.notify_one();
    }
};

SimpleSemaphore sem(3); // 3 permits max

void worker(int id) {
    sem.acquire(); // Decrements permit count
    std::cout << "Worker " << id << " entered critical quota.\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "Worker " << id << " leaving quota.\n";
    sem.release(); // Returns permit
}

int main() {
    std::vector<std::thread> threads;
    for (int i = 1; i <= 5; ++i) {
        threads.emplace_back(worker, i);
    }
    for (auto& t : threads) {
        t.join();
    }
    return 0;
}