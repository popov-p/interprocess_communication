#include "timed_ops.hpp"
TimedSemaphore::TimedSemaphore(int init_value)  {
    if (sem_init(&semaphore_, 1, init_value) == -1) {
        syslog(LOG_ERR, "Semaphore initialization error");
        exit(EXIT_FAILURE);
    }
}

void TimedSemaphore::wait(int timeout_seconds) {
    struct timespec current_time;
    if (clock_gettime(CLOCK_REALTIME, &current_time) == -1) {
        syslog(LOG_ERR, "Error getting current time");
        exit(EXIT_FAILURE);
    }
    struct timespec expiration_time;
    expiration_time.tv_sec = current_time.tv_sec + timeout_seconds;
    expiration_time.tv_nsec = 0;
    int result = sem_timedwait(&semaphore_, &expiration_time);
    if (result == 0) {
        syslog(LOG_INFO, "Semaphore acquired successfully!");
    } else if (result == -1) {
        syslog(LOG_ERR, "Semaphore wait timed out");
        exit(EXIT_FAILURE);
    }
}

void TimedSemaphore::post() {
        sem_post(&semaphore_);
}

TimedSemaphore::~TimedSemaphore() {
    sem_destroy(&semaphore_);
}

bool timer(int fd, int timeout_seconds, OperationType type) {
    fd_set io_set;
    FD_ZERO(&io_set);
    FD_SET(fd, &io_set);

    struct timeval timeout;
    timeout.tv_sec = timeout_seconds;
    timeout.tv_usec = 0;

    int result;

    if (type == OperationType::Read) {
        result = select(fd + 1, &io_set, nullptr, nullptr, &timeout);
    } else {
        result = select(fd + 1, nullptr, &io_set, nullptr, &timeout);
    }
    if (result == -1) {
        syslog(LOG_ERR, "Select error");
        return false;
    } else if (result == 0) {
        syslog(LOG_ERR, "I/O timeout");
        return false;
    } else {
        syslog(LOG_INFO, "Timer: ok");
        return true;
    }
}
