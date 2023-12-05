#include "../conn.h"
#ifndef MMAP_H
#define MMAP_H
class Mmap : public Conn {
    public:
    Mmap() {};
    Mmap(bool is_parent){};
    bool read(void *buf, size_t count){return true;};
    bool write(void *buf, size_t count){return true;};
    ~Mmap() {};
};
#endif