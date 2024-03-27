#pragma once

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
