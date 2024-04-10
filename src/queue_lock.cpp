#include <condition_variable>
#include <mutex>
#include <queue>

template <typename T>
class ThreadSafeQueue {
  private:
    std::queue<T> queue;
    mutable std::mutex mutex;
    std::condition_variable cv;

  public:
    void enqueue(const T &item) {
        {
            std::lock_guard<std::mutex> lock(mutex);
            queue.push(item);
        }
        cv.notify_one();
    }

    T dequeue() {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock, [this] { return !queue.empty(); });
        T item = queue.front();
        queue.pop();
        return item;
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex);
        return queue.size();
    }
};

#include <iostream>
#include <thread>

void producer(ThreadSafeQueue<int> &queue) {
    for (int i = 0; i < 5; ++i) {
        queue.enqueue(i);
    }
}

void consumer(ThreadSafeQueue<int> &queue) {
    while (queue.size() > 0) {
        int item = queue.dequeue();
        std::cout << "Consumed: " << item << std::endl;
    }
}

int main() {
    ThreadSafeQueue<int> queue;

    // 创建生产者线程
    std::thread producer_thread(producer, std::ref(queue));

    // 创建消费者线程
    std::thread consumer_thread(consumer, std::ref(queue));

    // 等待线程结束
    producer_thread.join();
    consumer_thread.join();

    return 0;
}
