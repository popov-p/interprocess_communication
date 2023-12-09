#include <cstddef>

#ifndef CONN_H
#define CONN_H

class Conn {
public:
    virtual bool read(void *buf, size_t count) = 0;
    virtual bool write(void *buf, size_t count) = 0;
    virtual ~Conn() = default;
};

#endif
