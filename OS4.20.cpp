#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>

std::mutex forks[2];

void dine(int id, int left_id, int right_id) {
    int first = std::min(left_id, right_id);
    int second = std::max(left_id, right_id);

    std::unique_lock<std::mutex> l1(forks[first]);
    std::unique_lock<std::mutex> l2(forks[second]);

    std::cout << "Philosopher " << id << " is eating using forks " << first << " and " << second << ".\n";
}

int main() {
    std::thread p1(dine, 1, 0, 1);
    std::thread p2(dine, 2, 0, 1);
    p1.join();
    p2.join();
    return 0;
}