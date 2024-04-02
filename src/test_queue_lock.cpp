#include <iostream>
#include <thread>

#include "queue_lock.h"

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
