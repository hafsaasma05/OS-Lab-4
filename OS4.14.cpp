#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

// Dev-C++ compatible custom Latch class
class SimpleLatch {
    std::mutex mtx;
    std::condition_variable cv;
    int count;
public:
    explicit SimpleLatch(int count_) : count(count_) {}

    void count_down() {
        std::unique_lock<std::mutex> lock(mtx);
        if (--count <= 0) {
            cv.notify_all();
        }
    }

    void wait() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return count <= 0; });
    }
};

SimpleLatch start_flag(1);

void worker(int id) {
    std::cout << "Worker " << id << " waiting for start signal...\n";
    start_flag.wait();
    std::cout << "Worker " << id << " running benchmarks!\n";
}

int main() {
    std::thread w1(worker, 1);
    std::thread w2(worker, 2);

    std::cout << "Coordinator preparing shared memory...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    start_flag.count_down(); // Unblocks all waiting workers simultaneously
    
    w1.join();
    w2.join();
    return 0;
}