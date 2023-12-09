#include <iostream>
#include <unistd.h>
#include <semaphore.h>
#include <ctime>
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

#endif
