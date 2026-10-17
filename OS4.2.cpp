#include <iostream>
#include <thread>
#include <functional>

void worker(int val, int& ref) {
    val = val + 10;
    ref = ref + 10;
    std::cout << "Inside worker -> val (copy): " << val << ", ref (original updated): " << ref << std::endl;
}

int main() {
    int local_x = 5;
    int local_y = 20;

    std::cout << "Before thread -> local_x: " << local_x << ", local_y: " << local_y << std::endl;
    
    // std::ref wraps the variable into a reference_wrapper
    std::thread t(worker, local_x, std::ref(local_y));
    t.join();

    std::cout << "After thread -> local_x (unchanged): " << local_x << ", local_y (modified): " << local_y << std::endl;
    return 0;
}