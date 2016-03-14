#ifndef __BOARD_H__
#define __BOARD_H__

#include <unordered_set>
#include "checker.h"
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

const int vectors[2][2][2] = {
    {
        {-1, 1}, //only for player 0
        {1, 1},  //only for player 0
    },
    {
        {-1, -1},  //only for player 1
        {1, -1},   //only for player1
    },
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

#endif //__GAME_H__
