#include <cstdlib>
#include <unistd.h>
#include <ctime>
#include <poll.h> 
#include <cstring>
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
    int status_ = false;
public:
    static Goat& get_instance() {
        static Goat instance;
        return instance;
    }
    int get_status();
    void set_status(int s);
    int throw_number();
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
    int throw_number();
    int read_num_from_console();
};
#endif