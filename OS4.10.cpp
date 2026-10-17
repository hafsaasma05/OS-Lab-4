#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtxA, mtxB;

void transfer_funds() {
    // std::lock deadlock-free tarike se multiple mutex acquire karta hai (C++11 compatible)
    std::lock(mtxA, mtxB);
    
    // adopt_lock batata hai ke mutex pehle hi lock ho chuke hain
    std::lock_guard<std::mutex> lockA(mtxA, std::adopt_lock);
    std::lock_guard<std::mutex> lockB(mtxB, std::adopt_lock);
    
    std::cout << "Successfully acquired both locks atomically without deadlock.\n";
}

int main() {
    std::thread t1(transfer_funds);
    std::thread t2(transfer_funds);
    
    t1.join();
    t2.join();
    
    return 0;
}