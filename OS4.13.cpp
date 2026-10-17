#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

// Dev-C++ compatible custom Cyclic Barrier
class SimpleBarrier {
    std::mutex mtx;
    std::condition_variable cv;
    int threshold;
    int count;
    int generation;
public:
    explicit SimpleBarrier(int count_) : threshold(count_), count(count_), generation(0) {}

    void arrive_and_wait() {
        std::unique_lock<std::mutex> lock(mtx);
        int gen = generation;
        if (--count == 0) {
            generation++;
            count = threshold;
            cv.notify_all();
        } else {
            cv.wait(lock, [this, gen] { return gen != generation; });
        }
    }
};

SimpleBarrier sync_point(3); // Wait for 3 threads

void worker(int id) {
    std::cout << "Thread " << id << " reached phase 1.\n";
    sync_point.arrive_and_wait(); // Wait for all participants
    std::cout << "Thread " << id << " proceeding to phase 2.\n";
}

int main() {
    std::thread t1(worker, 1);
    std::thread t2(worker, 2);
    std::thread t3(worker, 3);
    
    t1.join();
    t2.join();
    t3.join();
    
    return 0;
}