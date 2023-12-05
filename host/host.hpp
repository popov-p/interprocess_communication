#include <optional>

#ifndef HOST_H
#define HOST_H

template <typename Connection>
class Wolf {
private:
    Wolf() = default;
    Wolf(const Wolf&) = delete;
    Wolf(Wolf&&) = delete;
    Wolf& operator=(const Wolf&) = delete;
    Wolf& operator=(Wolf&&) = delete;

    void init_() const {
        //Connection conn;
        //Connection gw_pipe(true);

        pid_t goat_pid = fork();
//TODO: signal handling    
        if (goat_pid < 0) {
            exit(EXIT_FAILURE);
        }
        else if (goat_pid) {
            //parent behavior
        }
        else {
            //child behavior
        }
    };
    Connection w_g_connection;
    std::optional<Connection> g_w_connection; //for pipe
public:
    void run() const;
    static Wolf& get_instance() {
        static Wolf<Connection> instance;
        return instance;
    }
    ~Wolf() = default;
};

class Goat {
private:
    Goat() = default;
    Goat(const Goat&) = delete;
    Goat(Goat&&) = delete;
    Goat& operator=(const Goat&) = delete;
    Goat& operator=(Goat&&) = delete;
public:
    static Goat& get_instance() {
        static Goat instance = Goat();
        return instance;
    }
    ~Goat() = default;
};
#endif