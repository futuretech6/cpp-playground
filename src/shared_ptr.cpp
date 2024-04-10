#include <mutex>

template <class T>
class MySharedPtr {
  private:
    T *p;
    int *pRefCount;
    std::mutex *pMutex;

  public:
    MySharedPtr(T *ptr = nullptr) : p(ptr), pRefCount(new int(1)), pMutex(new std::mutex) {}

    ~MySharedPtr() { sub_ref_count(); }

    MySharedPtr(MySharedPtr<T> const &that) : p(that.p), pRefCount(that.pRefCount), pMutex(that.pMutex) {
        add_ref_count();
    }

    MySharedPtr<T> &operator=(const MySharedPtr<T> &that) {
        if (p != that.p) {
            sub_ref_count();
            p         = that.p;
            pRefCount = that.pRefCount;
            pMutex    = that.pMutex;
            add_ref_count();
        }
        return *this;
    }

    T &operator*() { return *p; }

    T *operator->() { return p; }

  private:
    void add_ref_count() {
        pMutex->lock();
        ++(*pRefCount);
        pMutex->unlock();
    }
    void sub_ref_count() {
        bool delete_flag = false;

        pMutex->lock();  // avoid double free
        if (--(*pRefCount) == 0) {
            delete pRefCount;
            delete p;
            delete_flag = true;
        }
        pMutex->unlock();

        if (delete_flag == true) {
            delete pMutex;
        }
    }
};

#include <iostream>

struct Test {
    int value;
    Test(int val) : value(val) {}
    void print() { std::cout << "Value: " << value << std::endl; }
};

int main() {
    // 测试构造函数和赋值运算符重载
    MySharedPtr<Test> ptr1(new Test(5));
    MySharedPtr<Test> ptr2(ptr1);
    MySharedPtr<Test> ptr3 = ptr1;

    // 测试解引用和成员访问运算符重载
    (*ptr1).print();
    ptr2->print();

    // 测试指针交换
    ptr1 = ptr3;

    // 测试自引用
    ptr1 = ptr1;

    return 0;
}
