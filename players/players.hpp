#ifndef PLAYERS_H
#define PLAYERS_H

template <typename PlayerType>
class Player {
private:
    Player() = default;
    Player(const Player&) = delete;
    Player(Player&&) = delete;
    Player& operator=(const Player&) = delete;
    Player& operator=(Player&&) = delete;
public:
    static PlayerType& get_instance() {
        static PlayerType instance;
        return instance;
    };
    virtual void throw_number() = 0;
    virtual ~Player() = default;
};

class Goat : public Player<Goat> {
private:
    bool status_;
public:
    void throw_number() {};
    ~Goat() = default;
};

class Wolf : public Player<Wolf> {
public:
    void throw_number(){};
    void read_num_from_console() {};
};

#endif