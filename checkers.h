#ifndef __CHECKERS_H__
#define __CHECKERS_H__
#include <vector>

using namespace std;

#define NUM_CHECKERS 12
#define NUM_COLS 8
#define NUM_ROWS 8
#define NUM_PLAYERS 2

const int player0_start_x[NUM_CHECKERS] = {1, 3, 5, 7, 0, 2, 4, 6, 1, 3, 5, 7};
const int player0_start_y[NUM_CHECKERS] = {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2};

const int player1_start_x[NUM_CHECKERS] = {0, 2, 4, 6, 1, 3, 5, 7, 0, 2, 4, 6};
const int player1_start_y[NUM_CHECKERS] = {7, 7, 7, 7, 6, 6, 6, 6, 5, 5, 5, 5};

const int player_symbols[NUM_PLAYERS] = {'X', 'O'};

class Checker {
    private:
        int player;
        int x_pos;
        int y_pos;
        bool alive;
    public:
        Checker(const int player, const int x_pos, const int y_pos);
        int get_player() const;
        void set_player(const int player);
        int get_x() const;
        int get_y() const;
        void set_x(const int);
        void set_y(const int);
        bool is_alive() const;
        void set_alive(const bool alive);
};

class Board {
    private:
        vector<Checker*> checkers;
    public:
        Board();
        Checker* get_checker_at(const int x, const int y) const;
        void kill_checker_at(const int x, const int y);
        void print_board() const;
        int player_checker_count(const int playernum) const;
        bool can_player_move(const int playernum) const;
        bool is_move_legal(const int x_start, const int y_start,
                const int x_finish, const int y_finish) const;
};

class Game {
    private:
        Board board;
    public:
        void get_player1_move();
        void get_player2_move();
};

#endif //__CHECKERS_H__
