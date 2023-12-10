#include <iostream>
#include <unistd.h>
#include <semaphore.h>
#include <ctime>
#include <cstring>
#include "../connections/conn.h"
#ifndef T_SEM_H
#define T_SEM_H

const int TIMEOUT_SECONDS = 5;
const int SLEEP_BETWEEN_ROUNDS = 6;
class TimedSemaphore {
public:
TimedSemaphore(int init_value = 1);
    void wait(int timeout_seconds);
    void post();
    ~TimedSemaphore();
private:
    sem_t semaphore_;
};

bool timed_io(int fd, void *buf, size_t count, int timeout_seconds, OperationType type); //for pipe
#endif