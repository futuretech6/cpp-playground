#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

int main() {
    std::mutex mtx;  // for child thread only
    std::condition_variable cv;

    std::thread threads[5];

    for (int i = 0; i < 5; ++i) {
        threads[i] = std::thread(
            [&](int id) {
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock);
                std::cout << "Thread " << id << " is working" << std::endl;
            },
            i + 1
        );
    }

    // main thread does some work
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::lock_guard<std::mutex> lock(mtx);  // avoid printing simultaneously with child-thread
        std::cout << "Notifying all threads..." << std::endl;
    }

    cv.notify_all();

    for (auto &thread : threads) {
        // cv.notify_one();
        thread.join();
    }

    return 0;
}
