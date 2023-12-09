#include <stdint.h>
#include <cstdlib>
#include <unistd.h>
#include <ctime> 
#ifndef PLAYERS_H
#define PLAYERS_H
enum Status {
    hidden = true,
    dead = false
};

class Goat {
    Goat(const Goat&) = delete;
    Goat(Goat&&) = delete;
    Goat& operator=(const Goat&) = delete;
    Goat& operator=(Goat&&) = delete;
private:
    Goat() = default;
    bool status_ = false;
    uint8_t buffer_; // buffer for receiving hidden or dead status
public:
    static Goat& get_instance() {
        static Goat instance;
        return instance;
    }
    bool get_status() {
        return status_;
    }
    void set_status(int s) {
        status_ = s;
    }
    int throw_number() {
        if (get_status() == Status::hidden) {
            std::srand(static_cast<unsigned int>(std::time(0)));
            return std::rand() % 100 + 1;
        }
        else /*dead*/{ 
            std::srand(static_cast<unsigned int>(std::time(0)));
            return std::rand() % 50 + 1;
        }
    }
    ~Goat() = default;
};

class Wolf {
    Wolf(const Wolf&) = delete;
    Wolf(Wolf&&) = delete;
    Wolf& operator=(const Wolf&) = delete;
    Wolf& operator=(Wolf&&) = delete;
private:
    Wolf() = default;
public:
    static Wolf& get_instance() {
        static Wolf instance;
        return instance;
    }
    int throw_number()  {
        std::srand(static_cast<unsigned int>(std::time(0)) + getpid());
        return std::rand() % 100 + 1;
    }
    int read_num_from_console() {
        return 0;
    }
};
#endif