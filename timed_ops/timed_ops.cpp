#include "timed_ops.hpp"
#include <cstring>
TimedSemaphore::TimedSemaphore(int init_value)  {
    if (sem_init(&semaphore_, 1, init_value) == -1) {
        std::cout << "semaphore initialization error" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void TimedSemaphore::wait(int timeout_seconds) {
    struct timespec current_time;
    if (clock_gettime(CLOCK_REALTIME, &current_time) == -1) {
        std::cout << "Error getting current time." << std::endl;
        exit(EXIT_FAILURE);
    }
    struct timespec expiration_time;
    expiration_time.tv_sec = current_time.tv_sec + timeout_seconds;
    expiration_time.tv_nsec = 0;
    int result = sem_timedwait(&semaphore_, &expiration_time);
    if (result == 0) {
        //std::cout << "Semaphore acquired successfully!" << std::endl;
    } else if (result == -1) {
        std::cout << "Semaphore wait timed out! errno: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
}

void TimedSemaphore::post() {
        sem_post(&semaphore_);
}

TimedSemaphore::~TimedSemaphore() {
    sem_destroy(&semaphore_);
}

bool timed_io(int fd, void *buf, size_t count, int timeout_seconds, OperationType type) {
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
        std::cerr << "Select error: " << strerror(errno) << std::endl;
        return false;
    } else if (result == 0) {
        std::cerr << (type == OperationType::Read ? "Read" : "Write") << " timeout" << std::endl;
        return false;
    } else {
        ssize_t bytes_processed = (type == OperationType::Read) ? ::read(fd, buf, count) : ::write(fd, buf, count);

        if (bytes_processed == -1) {
            std::cerr << (type == OperationType::Read ? "Read" : "Write") << " error: " << strerror(errno) << std::endl;
            return false;
        }
        return true;
    }
}
