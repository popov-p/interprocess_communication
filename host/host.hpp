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
template <typename Connection, template <typename> class PtrType>
class Host {
    Host(const Host&) = delete;
    Host(Host&&) = delete;
    Host& operator=(const Host&) = delete;
    Host& operator=(Host&&) = delete;
private:
    Host() {
        //openlog("interprocess_communication", LOG_PID, LOG_USER);
    };
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
        //syslog(LOG_INFO, "initial goat spawned");
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
                    int current_wolf_num;
                    if(keyboard_input_) {
                        current_wolf_num = wolf.read_num_from_console();
                    }
                    else {
                        current_wolf_num = wolf.throw_number();
                    }
                    int current_goat_num;
                    /*unwrap*/
                    if (!g_w_connection_->read(&current_goat_num, sizeof(int))){
                        //closelog();
                        exit(EXIT_FAILURE);
                    }
                    //g_w_connection_->read(&current_goat_num, sizeof(int));
                    if ((dead_times_ == 0) && (abs(current_wolf_num - current_goat_num)<= 50)) {
                        int status = Status::hidden;
                        if(!w_g_connection_->write(&status, sizeof(int))) {
                            //closelog();
                            exit(EXIT_FAILURE);
                        }
                    }
                    else if ((dead_times_ == 1) && (abs(current_wolf_num - current_goat_num) <= 30)) {
                        int status = Status::hidden;
                        if(!w_g_connection_->write(&status, sizeof(int))){
                            //closelog();
                            exit(EXIT_FAILURE);
                        } //reincarnation
                        dead_times_--;
                    }
                    else {
                        int status = Status::dead;
                        if(!w_g_connection_->write(&status, sizeof(int))){
                            //closelog();
                            exit(EXIT_FAILURE);
                        }
                        dead_times_++;
                        if(dead_times_ == 2) {
                            print_round_log(current_wolf_num, current_goat_num);
                            block_conniection();
                            //syslog(LOG_INFO, "game finished");
                            //closelog();
                            exit(EXIT_SUCCESS);
                        }
                    }
                    print_round_log(current_wolf_num, current_goat_num);
                    std::this_thread::sleep_for(std::chrono::seconds(SLEEP_BETWEEN_ROUNDS));
                }
            case 0: // child 
                while(true) {
                    int status;
                    if(!w_g_connection_->read(&status, sizeof(int))) {
                        //closelog();
                        exit(EXIT_FAILURE);
                    }
                    Goat& goat = Goat::get_instance();
                    goat.set_status(status);
                    int current_goat_num = goat.throw_number();
                    if(!g_w_connection_->write(&current_goat_num, sizeof(int))) {
                        //closelog();
                        exit(EXIT_FAILURE);
                    }
                    
                    std::this_thread::sleep_for(std::chrono::seconds(SLEEP_BETWEEN_ROUNDS));
                }
        }
    };
    void connect() {
        if constexpr (std::is_same_v<Connection, Mmap>) {
            w_g_connection_ = std::make_shared<Mmap>(sizeof(int));
            g_w_connection_ = w_g_connection_;
        }
        else if constexpr (std::is_same_v<Connection, Pipe>) {
            w_g_connection_ = std::make_unique<Pipe>();
            g_w_connection_ = std::make_unique<Pipe>();
        }
        else {
            //call shm consructor here
        }
    }
    
    PtrType<Connection> w_g_connection_;
    PtrType<Connection> g_w_connection_; //bidirectiobal pipe
    int round_counter_ = 0;
    int dead_times_ = 0;
    bool keyboard_input_ = false;
    //signals
    static void h_sigterm_(int sig) {
        std::cout << "SIGTERM RECEIVED" << std::endl;
        Host::get_instance().block_conniection();
	    exit(EXIT_FAILURE);
    }
    void block_conniection() {
        int st = ConnStatus::INACCESSIBLE;
        w_g_connection_->write(&st, sizeof(int));
        g_w_connection_->write(&st, sizeof(int));
        std::cout << "inaccessible status set" << std::endl;
    }
public:
    void set_keyboard_input_flag(bool flag) {
        keyboard_input_ = flag;
    }
    void run(const std::string& executable_name) {
        connect();
        const pid_t & goat_pid = fork();    
        if (goat_pid < 0) {
            exit(EXIT_FAILURE);
        }
        else {
            std::signal(SIGTERM, h_sigterm_);
            manage_game(goat_pid);
        }
    };
    static Host& get_instance() {
        static Host<Connection, PtrType> instance;
        return instance;
    }
    ~Host() = default;
};
#endif