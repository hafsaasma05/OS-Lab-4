#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

// Dev-C++ compatible stop token representation
class SimpleStopToken {
    std::atomic<bool>* flag;
public:
    explicit SimpleStopToken(std::atomic<bool>* f) : flag(f) {}
    bool stop_requested() const {
        return flag && flag->load();
    }
};

// Dev-C++ compatible jthread wrapper with stop support
class SimpleJThread {
    std::thread t;
    std::atomic<bool> stop_flag{false};
public:
    template<typename F>
    explicit SimpleJThread(F f) {
        stop_flag.store(false);
        t = std::thread([this, f]() {
            SimpleStopToken token(&this->stop_flag);
            f(token);
        });
    }

    void request_stop() {
        stop_flag.store(true);
    }

    ~SimpleJThread() {
        request_stop();
        if (t.joinable()) {
            t.join();
        }
    }

    SimpleJThread(const SimpleJThread&) = delete;
    SimpleJThread& operator=(const SimpleJThread&) = delete;
};

void worker_func(SimpleStopToken stoken) {
    while (!stoken.stop_requested()) {
        std::cout << "Worker working...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "Stop requested, cleaning up resources.\n";
}

int main() {
    SimpleJThread my_jthread(worker_func);
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    my_jthread.request_stop();
    return 0;
}