#include <atomic>
#include <memory>

template <typename T>
class LockFreeQueue {
  private:
    struct Node {
        std::shared_ptr<T> data;
        Node *next;

        Node(const T &value) : data(std::make_shared<T>(value)), next(nullptr) {}
    };

    std::atomic<Node *> head;
    std::atomic<Node *> tail;

  public:
    LockFreeQueue() : head(new Node(T())), tail(head.load()) {}

    ~LockFreeQueue() {
        while (Node *old_head = head.load()) {
            head.store(old_head->next);
            delete old_head;
        }
    }

    void enqueue(const T &value) {
        Node *new_node = new Node(value);
        Node *old_tail = tail.exchange(new_node);
        old_tail->next = new_node;
    }

    std::shared_ptr<T> dequeue() {
        Node *old_head = head.load();
        while (true) {
            if (old_head == tail.load()) {
                if (old_head->next == nullptr) {
                    return nullptr;  // Queue is empty
                }
                tail.compare_exchange_weak(old_head, old_head->next);
            } else {
                std::shared_ptr<T> res(old_head->next->data);
                head.store(old_head->next);
                delete old_head;
                return res;
            }
        }
    }
};

#include <iostream>
#include <thread>

// 示例用法
int main() {
    LockFreeQueue<int> queue;

    // 生产者线程
    std::thread producer([&]() {
        for (int i = 0; i < 5; ++i) {
            queue.enqueue(i);
        }
    });

    // 消费者线程
    std::thread consumer([&]() {
        for (int i = 0; i < 5; ++i) {
            std::shared_ptr<int> item = queue.dequeue();
            if (item) {
                std::cout << "Consumed: " << *item << std::endl;
            }
        }
    });

    producer.join();
    consumer.join();

    return 0;
}
