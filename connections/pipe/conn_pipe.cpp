#include <conn_pipe.hpp>
#include <iostream>
#include <cstring>
#include <unistd.h>
//TODO: error processing add here


Pipe::Pipe() : sem_(1) {
    if (pipe(pipe_descriptors_) == -1) {
        syslog(LOG_ERR, "Pipe initialization error");
        exit(EXIT_FAILURE);
    }
}

bool Pipe::read(void *buf, size_t count) {
    sem_.wait(TIMEOUT_SECONDS);
    if (timer(pipe_descriptors_[PipeEnd::READ_END],TIMEOUT_SECONDS, OperationType::Read)) {
        ssize_t bytes_processed = ::read(pipe_descriptors_[PipeEnd::READ_END], buf, count);
        if (bytes_processed == -1) {
            syslog(LOG_ERR, "Pipe read error");
            sem_.post();
            return false;
        }
        else if (*(static_cast<int*>(buf)) == ConnStatus::INACCESSIBLE) {
            syslog(LOG_ERR, "Pipe connection internally closed");
            return false;            
        } else {
            syslog(LOG_INFO, "Pipe read: ok");
            sem_.post();
            return true;
        }
    } else {
        sem_.post();
        return false;
    }

}

bool Pipe::write(void *buf, size_t count) {
    sem_.wait(TIMEOUT_SECONDS);
    if (timer(pipe_descriptors_[PipeEnd::WRITE_END],TIMEOUT_SECONDS, OperationType::Write)) {
        ssize_t bytes_processed = ::write(pipe_descriptors_[PipeEnd::WRITE_END], buf, count);
        if (bytes_processed == -1) {
            syslog(LOG_ERR, "Pipe write error");
            sem_.post();
            return false;
        }
        else if (*(static_cast<int*>(buf)) == ConnStatus::INACCESSIBLE) {
            syslog(LOG_ERR, "Pipe connection internally closed");
            sem_.post();
            return false;            
        }
        else {
            syslog(LOG_INFO, "Pipe write: ok");
            sem_.post();
            return true;
        }
    } else {
        sem_.post();
        return false;
    }
};

Pipe::~Pipe() {
    syslog(LOG_INFO, "Pipe destructor called");
    sem_.~TimedSemaphore();
    close(pipe_descriptors_[READ_END]);
    close(pipe_descriptors_[WRITE_END]);
    closelog();
}