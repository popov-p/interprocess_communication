#include <semaphore.h>
#include "../conn.h"

#ifndef CONN_PIPE_H
#define CONN_PIPE_H

enum PipeEnd {
    READ_END = 0,
    WRITE_END = 1
};

class Pipe : public Conn {
public:
    Pipe() {};
    Pipe(bool is_parent);
    bool read(void *buf, size_t count) override;
    bool write(void *buf, size_t count) override;
    ~Pipe();
private:
    int pipe_descriptors_[2];
    sem_t semaphore_;
    bool is_parent_process_;
};

#endif