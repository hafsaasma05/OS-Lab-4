#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex mtxA, mtxB;

void thread1_func() {
    std::lock_guard<std::mutex> lockA(mtxA);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Thread 1 acquired mtxA, waiting for mtxB...\n";
    std::lock_guard<std::mutex> lockB(mtxB); 
    std::cout << "Thread 1 acquired both locks.\n";
}

void thread2_func() {
    std::lock_guard<std::mutex> lockB(mtxB);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Thread 2 acquired mtxB, waiting for mtxA...\n";
    std::lock_guard<std::mutex> lockA(mtxA); 
    std::cout << "Thread 2 acquired both locks.\n";
}

int main() {
    std::thread t1(thread1_func);
    std::thread t2(thread2_func);
    t1.join();
    t2.join();
    return 0;
}