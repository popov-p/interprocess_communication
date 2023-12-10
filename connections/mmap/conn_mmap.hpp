#include "../conn.h"
#include "../../timed_ops/timed_ops.hpp"
#include <sys/mman.h>
#ifndef MMAP_H
#define MMAP_H
class Mmap : public Conn {
    public:
    Mmap(size_t type_size);
    Mmap(bool is_parent){};
    bool read(void *buf, size_t count);
    bool write(void *buf, size_t count);
    ~Mmap();
private:
    TimedSemaphore sem_;
    int *shared_data_ = nullptr;
    size_t mem_size_;
};
#endif