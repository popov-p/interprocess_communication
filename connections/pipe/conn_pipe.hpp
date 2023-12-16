#include "../conn.h"
#include "../../timed_ops/timed_ops.hpp"
#ifndef CONN_PIPE_H
#define CONN_PIPE_H

enum PipeEnd {
    READ_END = 0,
    WRITE_END = 1
};

class Pipe : public Conn {
public:
    Pipe();
    bool read(void *buf, size_t count) override;
    bool write(void *buf, size_t count) override;
    ~Pipe();
private:
    int pipe_descriptors_[2];
    TimedSemaphore sem_;
};

#endif