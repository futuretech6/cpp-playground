#include <iostream>
#include <thread>

#include "queue_lockfree.h"

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
