#include <cstddef>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

template <typename T>
class MsgQueue {
  private:
    std::queue<T> dataQueue;
    std::mutex mtx;
    int const maxBatchSize;  // number of elem can be pushed at one lock-period

  public:
    MsgQueue(int maxBatchSize) : maxBatchSize(maxBatchSize) {}

    void push(std::vector<T> const &inputData) {
        for (size_t i = 0; i < inputData.size();) {
            std::lock_guard<std::mutex> lock(mtx);
            for (auto count = maxBatchSize; count && i < inputData.size(); count--) {
                dataQueue.push(inputData[i++]);
            }
        }
    }

    std::vector<T> pop(size_t num) {
        std::vector<T> data_vec;
        for (; num && !dataQueue.empty(); num--) {
            data_vec.push_back(dataQueue.front());
            dataQueue.pop();
        }
        return std::move(data_vec);
    }
};

int main() {
    MsgQueue<int> msg_queue(3);

    std::thread thread1([&msg_queue]() { msg_queue.push({1, 2, 3, 4, 5}); });
    std::thread thread2([&msg_queue]() { msg_queue.push({6, 7, 8, 9, 10}); });

    thread1.join();
    thread2.join();

    for (auto &i : msg_queue.pop(10)) {
        std::cout << i << std::endl;
    }

    return 0;
}
