#include <iostream>
#include <thread>

thread_local int txn_id = 0; // Each thread has its own distinct copy

void run(int id) {
    txn_id += id;
    std::cout << "Thread " << id << " txn_id: " << txn_id << std::endl;
}

int main() {
    std::thread t1(run, 10);
    std::thread t2(run, 20);
    t1.join();
    t2.join();
    return 0;
}