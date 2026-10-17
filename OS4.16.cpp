#include <iostream>
#include <future>

int heavy_calc(int arg) {
    return arg * arg;
}

int main() {
    std::future<int> fut = std::async(std::launch::async, heavy_calc, 10);
    std::cout << "Doing concurrent work in main()...\n";
    
    int res = fut.get();
    std::cout << "Result from async task: " << res << std::endl;
    return 0;
}