#include <optional>
#include <thread>
#include <memory>
#include <csignal>
#include "../connections/mmap/conn_mmap.hpp"
#include "../connections/shm/conn_shm.hpp"
#include "../connections/pipe/conn_pipe.hpp"
#include "../players/players.hpp"

#ifndef HOST_H
#define HOST_H
//TODO: signal handling
template <typename Connection>
class Host {
    Host(const Host&) = delete;
    Host(Host&&) = delete;
    Host& operator=(const Host&) = delete;
    Host& operator=(Host&&) = delete;
private:
    Host() = default;
    void print_round_log(const int& current_wolf_num, const int& current_goat_num) {
        std::cout << "Current round: " << round_counter_ << std::endl;
        std::cout << "Wolf threw: " << current_wolf_num << std::endl;
        std::cout << "Goat threw: " << current_goat_num << std::endl;
        if(dead_times_ == 0) {
            std::cout << "Number of alive goats: " << 1 << std::endl;
        }
        else {
            std::cout << "Number of alive goats: " << 0 << std::endl;
        }
        if(dead_times_ == 2) {
            std::cout << "--- GAME FINISHED ---" <<std::endl;
        }
        
    }
    void spawn_goat() {
        int status = Status::hidden;
        w_g_connection_->write(&status, sizeof(int));
    }    
    void manage_game(const pid_t& pid) {
        switch (pid) {
            default:
                while(true) {
                    round_counter_++;
                    if(round_counter_ == 1) {
                        spawn_goat();
                    }
                    Wolf& wolf = Wolf::get_instance();
                    int current_wolf_num = wolf.throw_number();
                    int current_goat_num;

                    g_w_connection_.value()->read(&current_goat_num, sizeof(int));
                    if ((dead_times_ == 0) && (abs(current_wolf_num - current_goat_num)<= 50)) {
                        int status = Status::hidden;
                        w_g_connection_->write(&status, sizeof(int));
                    }
                    else if ((dead_times_ == 1) && (abs(current_wolf_num - current_goat_num) <= 30)) {
                        int status = Status::hidden;
                        w_g_connection_->write(&status, sizeof(int)); //reincarnation
                        dead_times_--;
                    }
                    else {
                        int status = Status::dead;
                        w_g_connection_->write(&status, sizeof(int));
                        dead_times_++;
                        if(dead_times_ == 2) {
                            print_round_log(current_wolf_num, current_goat_num);
                            exit(EXIT_SUCCESS);
                        }
                    }
                    print_round_log(current_wolf_num, current_goat_num);
                    std::this_thread::sleep_for(std::chrono::seconds(5));
                }
            case 0: // child 
                while(true) {
                        int status;
                        w_g_connection_->read(&status, sizeof(int));
                        Goat& goat = Goat::get_instance();
                        goat.set_status(status);
                        int current_goat_num = goat.throw_number();
                        g_w_connection_.value()->write(&current_goat_num, sizeof(int));
                        std::this_thread::sleep_for(std::chrono::seconds(5));
                }
        }
    };
    
    void connect() {
        if constexpr (std::is_same_v<Connection, Mmap>) {
        //call mmap constructor here
        }
        else if constexpr (std::is_same_v<Connection, Pipe>) {
            w_g_connection_ = std::make_unique<Pipe>();
            g_w_connection_ = std::make_unique<Pipe>();
        }
        else {
        //call shm consructor here
        }
    }
    std::unique_ptr<Connection> w_g_connection_;
    std::optional<std::unique_ptr<Connection>> g_w_connection_; //for pipe
    int round_counter_ = 0;
    int dead_times_ = 0;
    //signals
    void h_sigterm_(int sig) {
        std::cout << "SIGTERM RECEIVED" << std::endl;
	    exit(EXIT_SUCCESS);
    }


public:
    void run(const std::string& executable_name) {
        connect();
        const pid_t & goat_pid = fork();    
        if (goat_pid < 0) {
            exit(EXIT_FAILURE);
        }
        else {
            manage_game(goat_pid);
        }
    };
    static Host& get_instance() {
        static Host<Connection> instance;
        return instance;
    }
    ~Host() = default;
};
#endif