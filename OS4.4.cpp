#include <iostream>
#include <thread>
#include <utility>

void worker_fn() {
    std::cout << "Worker thread executing safely via RAII.\n";
}

// Custom RAII wrapper jo std::jthread ki tarah kaam karta hai (GCC 9 compatible)
class MyJThread {
    std::thread t;
public:
    template<typename... Args>
    explicit MyJThread(Args&&... args) : t(std::forward<Args>(args)...) {}

    ~MyJThread() {
        if (t.joinable()) {
            t.join(); // Automatically calls join on destruction (scope exit)
        }
    }

    // Copy constructor/assignment delete karna zaroori hai
    MyJThread(const MyJThread&) = delete;
    MyJThread& operator=(const MyJThread&) = delete;
};

int main() {
    {
        std::cout << "Entering scope...\n";
        MyJThread jt(worker_fn); // Scope khatam hote hi automatic join ho jayega
    } // <- Yahan destructor call hoga aur thread join ho jayegi
    
    std::cout << "Scope exited safely with automatic join.\n";
    return 0;
}