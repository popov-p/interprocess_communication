#include <iostream>
#include <unistd.h>
#include <semaphore.h>
#include <ctime>
#include <cstring>
#include "../connections/conn.h"
#ifndef T_SEM_H
#define T_SEM_H


class TimedSemaphore {
public:
TimedSemaphore(int init_value = 1);
    void wait(int timeout_seconds);
    void post();
    ~TimedSemaphore();
private:
    sem_t semaphore_;
};

bool timed_io(int fd, void *buf, size_t count, int timeout_seconds, OperationType type);
//bool timed_read(int fd, void *buf, size_t count, int timeout_seconds);
//bool timed_write(int fd, void *buf, size_t count, int timeout_seconds);
#endif