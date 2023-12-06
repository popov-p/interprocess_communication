#include <stdint.h>
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
    static Goat& get_instance() {
        static Goat instance;
        return instance;
    }
public:
    bool get_status() {
        return status_;
    }
    void set_status(bool s) {
        status_ = s;
    }
    int throw_number() {
        return 0;
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
    static Wolf& get_instance() {
        static Wolf instance;
        return instance;
    }

public:
    void throw_number()  {}
    int read_num_from_console() {
        return 0;
    }
};
#endif