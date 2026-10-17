#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

std::mutex mtx;
int counter = 0;

void safe_increment() {
    for (int i = 0; i < 10000; ++i) {
        std::lock_guard<std::mutex> lock(mtx); // RAII lock acquisition
        counter++; // Critical section protected
    }
}

int main() {
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(safe_increment);
    }
    for (auto& t : threads) {
        t.join();
    }
    std::cout << "Expected counter value: 50000\n";
    std::cout << "Actual counter value (protected by mutex): " << counter << std::endl;
    return 0;
}