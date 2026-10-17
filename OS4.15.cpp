#include <iostream>
#include <thread>
#include <future>
#include <chrono>

void worker(std::promise<int> prom) {
    std::cout << "Worker computing value...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    prom.set_value(42);
}

int main() {
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();

    std::thread t(worker, std::move(prom));

    std::cout << "Master waiting for future value...\n";
    int result = fut.get(); // Blocks until value is ready
    std::cout << "Received result: " << result << std::endl;

    t.join();
    return 0;
}