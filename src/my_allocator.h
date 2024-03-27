

#include <cstddef>
#include <utility>

template <typename _Tp>
class MyAllocator {
  public:
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef _Tp *pointer;
    typedef const _Tp *const_pointer;
    typedef _Tp &reference;
    typedef const _Tp &const_reference;
    typedef _Tp value_type;

    template <typename _Tp1>
    struct rebind {
        typedef MyAllocator<_Tp1> other;
    };

    MyAllocator() {}
    MyAllocator(const MyAllocator &) noexcept {}
    template <typename _Tp1>
    MyAllocator(const MyAllocator<_Tp1> &) noexcept {}
    ~MyAllocator() noexcept {}

    pointer allocate(size_type __n, const void * = static_cast<const void *>(0));

    void deallocate(pointer __p, size_type);

    size_type max_size() const noexcept;

    template <typename _Up, typename... _Args>
    void construct(_Up *__p, _Args &&...__args) noexcept(noexcept(::new((void *)__p) _Up(std::forward<_Args>(__args)...)
    ));
    template <typename _Up>
    void destroy(_Up *__p) noexcept(noexcept(__p->~_Up()));
    //...
};
