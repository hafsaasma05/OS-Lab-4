#include <iostream>
#include <thread>
#include <chrono>

void logger() {
    for (int i = 0; i < 3; ++i) {
        std::cout << "[Logger Daemon] Flushing buffer to disk...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int main() {
    std::thread t(logger);
    t.detach(); // OS manages PCB/TCB deallocation upon thread termination
    
    std::cout << "Main thread sleeping briefly to let daemon run...\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Main thread exiting.\n";
    return 0;
}