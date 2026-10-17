#include <iostream>
#include <thread>
#include <vector>

int counter = 0;

void increment_routine() {
    for (int i = 0; i < 10000; ++i) {
        counter++; // Data race condition occurs here across concurrent threads
    }
}

int main() {
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(increment_routine);
    }
    for (auto& t : threads) {
        t.join();
    }
    std::cout << "Expected counter value: 50000\n";
    std::cout << "Actual counter value (likely incorrect due to data race): " << counter << std::endl;
    return 0;
}