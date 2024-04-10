#include <cassert>
#include <cmath>
#include <stdexcept>
#include <vector>

class BuddyAllocator {
  private:
    void *memory;
    std::vector<bool> isFree;
    int totalSize;
    int blockSize;  // size of min block
    int blockNum;   // num of min block, must be pow-of-2

    static inline int get_left_child(int index) { return 2 * index + 1; }

    static inline int get_right_child(int index) { return 2 * index + 2; }

    static inline int get_parent(int index) { return (index - 1) / 2; }

    static inline int get_current_depth(int index) { return floor(log2(index + 1)); }

    inline int get_size_of_block(int index) const { return this->totalSize / (1 << get_current_depth(index)); }

    inline void *index_to_ptr(int index) const {
        auto const block_index_size     = get_size_of_block(index);
        auto const current_level_offset = index + 1 - (1 << get_current_depth(index));
        return reinterpret_cast<char *>(memory) + (block_index_size * current_level_offset);
    }

    inline int ptr_to_index(void *ptr) {
        for (int index = 0, block_size = totalSize; index < isFree.size(); block_size /= 2) {
            auto const *index_ptr = index_to_ptr(index);
            assert(index_ptr <= ptr);
            if (index_ptr == ptr) {
                if (isFree[index] == false) {
                    return index;
                } else {
                    index = get_left_child(index);
                }
            } else if (index_ptr < ptr) {
                auto const *right_child_ptr = index_to_ptr(get_right_child(index));
                index                       = ptr < right_child_ptr ? get_left_child(index) : get_right_child(index);
            }
        }
        return -1;
    }

    int find_free_block(int index, int nBlocks) {
        if (index >= isFree.size()) {
            return -1;
        }
        if (nBlocks > blockNum) {
            return -1;
        }

        auto const block_index_size = get_size_of_block(index);

        assert((nBlocks & (nBlocks - 1)) == 0);  // nBlocks should be pow-of-2

        if (isFree[index] && block_index_size / blockSize == nBlocks) {
            return index;
        }

        if (auto left_index = find_free_block(get_left_child(index), nBlocks); left_index != -1) {
            return left_index;
        } else if (auto right_index = find_free_block(get_right_child(index), nBlocks); right_index != -1) {
            return right_index;
        } else {
            return -1;
        }
    }

  public:
    BuddyAllocator(int const blockSize, int const blockNum) {
        assert((blockNum & (blockNum - 1)) == 0);  // ensure blockNum is pow-of-2

        this->blockSize = blockSize;
        this->blockNum  = blockNum;
        this->totalSize = blockSize * blockNum;

        this->memory = new char[totalSize];
        this->isFree.resize(blockNum * 2 - 1, true);
    }

    ~BuddyAllocator() { delete[] reinterpret_cast<char *>(memory); }

    void *allocate(int size) {
        int const pow_of_two_blocks = pow(2, ceil(log2(size / blockSize + (size % blockSize ? 1 : 0))));

        if (auto index = find_free_block(0, pow_of_two_blocks); index != -1) {
            for (auto i = index;; i = get_parent(i)) {
                isFree[i] = false;  // ! node and all its ancestors are set, all its descendants aren't set
                if (i == 0) {
                    break;
                }
            }
            return index_to_ptr(index);
        } else {
            return nullptr;
        }
    }

    void deallocate(void *ptr) {
        if (int index = ptr_to_index(ptr); index != -1) {
            isFree[index] = true;
            for (auto i = get_parent(index);; i = get_parent(i)) {
                if (isFree[i]) {
                    break;
                }
                if (isFree[get_left_child(i)] && isFree[get_right_child(i)]) {
                    isFree[i] = true;  // ! merge free blocks
                }
                if (i == 0) {
                    break;
                }
            }
        } else {
            throw std::invalid_argument("invalid ptr");
        }
    }
};

#include <iostream>

int main() {
    using namespace std;

    BuddyAllocator allocator(4096, 8);       // Allocate 8 * 4K bytes
    void *ptr1 = allocator.allocate(100);    // Allocate 100 bytes
    void *ptr2 = allocator.allocate(10000);  // Allocate 200 bytes

    cout << "ptr1: " << ptr1 << endl;
    cout << "ptr2: " << ptr2 << endl;

    allocator.deallocate(ptr1);
    cout << "ptr1 deallocated" << endl;
    allocator.deallocate(ptr2);
    cout << "ptr2 deallocated" << endl;

    return 0;
}
