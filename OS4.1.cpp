#include <iostream>
#include <thread>

void child_routine() {
    std::cout << "Child executing with unique thread ID: " << std::this_thread::get_id() << std::endl;
}

int main() {
    std::cout << "Main thread starting.\n";
    std::thread t(child_routine);
    t.join(); // Blocks calling thread until 't' finishes
    std::cout << "Main thread finished.\n";
    return 0;
}