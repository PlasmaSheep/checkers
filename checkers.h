#ifndef __CHECKERS_H__
#define __CHECKERS_H__
#include <iostream>
#include <unordered_set>

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

const int diagonal_offsets[2][2] = {
    {-1, -1},
    {-1, 1},
};    

class Checker {
    private:
        int player;
        int x_pos;
        int y_pos;
    public:
        Checker(const int player, const int x_pos, const int y_pos);
        int get_player() const;
        void set_player(const int player);
        int get_x() const;
        int get_y() const;
        void set_x(const int);
        void set_y(const int);
        void set_alive(const bool alive);
};

class Board {
    private:
        unordered_set<Checker*> checkers;
    public:
        Board();
        Checker* get_checker_at(const int x, const int y) const;
        void kill_checker_at(const int x, const int y);
        void print_board() const;
        int player_checker_count(const int playernum) const;
        bool can_player_move(const int playernum) const;
        bool is_move_legal(const int x_start, const int y_start,
                const int x_finish, const int y_finish,
                const int player) const;
        bool is_move_legal(const Checker& piece, const int vector, const int player) const;
        int get_capture_vector(const Checker& checker) const;
        bool is_move_capture(const Checker& checker, const int x_end,
                const int y_end) const;
        bool is_position_legal(const int x, const int y) const; 
        bool can_player_move_piece(const int x, const int y, const int player) const;
        bool can_player_move_piece(const Checker& checker, const int player) const;
        void make_random_move(int player);
        void make_capture(Checker& checker, const int vector);
};

class Game {
    private:
        Board board;
        void get_player0_move();
        void get_player1_move();
    public:
        Game();
        int play(); 
};

#endif //__CHECKERS_H__
