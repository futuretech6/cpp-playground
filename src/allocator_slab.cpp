#include <cstddef>
#include <iostream>
#include <vector>

class SlabAllocator {
  private:
    struct Slab {
        size_t size;  // size of space without Slab
        Slab *next;
        bool isFree;

        // Slab(size_t size, bool isFree, Slab *next) : size(size), isFree(isFree), next(next) {}
    };

    Slab *head;

    void *split_block(Slab *block, size_t size) {
        if (block->size <= size + sizeof(Slab)) {  // new block has no enough space
            return nullptr;
        }

        size_t remaining_size = block->size - size - sizeof(Slab);

        // Create a new block after the current block
        Slab *new_block   = reinterpret_cast<Slab *>(reinterpret_cast<char *>(block) + sizeof(Slab) + size);
        new_block->size   = remaining_size;
        new_block->isFree = true;
        new_block->next   = block->next;

        // Adjust the size of the current block
        block->size = size;
        block->next = new_block;

        return reinterpret_cast<void *>(block + 1);  // Skip the header
    }

    void merge_blocks() {
        Slab *current = head;
        while (current != nullptr && current->next != nullptr) {
            if (current->isFree && current->next->isFree) {
                current->size += current->next->size + sizeof(Slab);
                current->next = current->next->next;
            }
            current = current->next;
        }
    }

  public:
    SlabAllocator(size_t initialSize) {
        head         = reinterpret_cast<Slab *>(new char[initialSize]);
        head->size   = initialSize;
        head->isFree = true;
        head->next   = nullptr;
    }

    ~SlabAllocator() {
        // Slab *current = head;
        // while (current != nullptr) {
        //     Slab *next = current->next;
        //     delete current;
        //     current = next;
        // }
        delete[] head;
    }

    void *allocate(size_t size) {
        Slab *current = head;
        while (current != nullptr) {
            if (current->isFree && current->size >= size) {
                if (auto *new_current = split_block(current, size); new_current != nullptr) {
                    current->isFree = false;
                    return reinterpret_cast<void *>(current + 1);  // Skip the header
                }
            }
            current = current->next;
        }
        return nullptr;  // No suitable block found
    }

    void deallocate(void *ptr) {
        Slab *block   = reinterpret_cast<Slab *>(ptr) - 1;  // Move back to the header
        block->isFree = true;
        merge_blocks();
    }
};

int main() {
    SlabAllocator allocator(1024);

    std::vector<void *> pointers;

    // Allocate some memory
    for (int i = 0; i < 5; ++i) {
        void *ptr = allocator.allocate(100);
        if (ptr != nullptr) {
            pointers.push_back(ptr);
            std::cout << "Allocated slab at address: " << ptr << std::endl;
        } else {
            std::cout << "Failed to allocate memory!" << std::endl;
        }
    }

    // Deallocate memory
    for (void *ptr : pointers) {
        allocator.deallocate(ptr);
        std::cout << "Deallocated slab at address: " << ptr << std::endl;
    }

    return 0;
}
