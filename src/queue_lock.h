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
