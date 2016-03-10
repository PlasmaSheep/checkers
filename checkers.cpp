#include "checkers.h"
#include <cstdio>

Checker::Checker(const int player, const int x_pos, const int y_pos)
   :player(player),
    x_pos(x_pos),
    y_pos(y_pos),
    alive(true)
{}

int Checker::get_player() const {
    return player;
}

void Checker::set_player(const int player) {
    this->player = player;
}

int Checker::get_x() const {
    return x_pos;
}

int Checker::get_y() const {
    return y_pos;
}

void Checker::set_x(const int x_pos) {
    this->x_pos = x_pos;
}

void Checker::set_y(const int y_pos) {
    this->y_pos = y_pos;
}

bool Checker::is_alive() const {
    return alive;
}

void Checker::set_alive(const bool alive) {
    this->alive = alive;
}


Board::Board() {
    for(size_t i = 0; i < NUM_CHECKERS; i++) {
        Checker* checker0 = new Checker(0, player0_start_x[i],
                player0_start_y[i]);
        Checker* checker1 = new Checker(1, player1_start_x[i],
                player1_start_y[i]);
        checkers.push_back(checker0);
        checkers.push_back(checker1);
    }
}

Checker* Board::get_checker_at(const int x, const int y) const {
    for(Checker* checker : checkers) {
        if(checker->get_x() == x and checker->get_y() == y) {
            return checker;
        }
    }
    return nullptr;
}

void Board::kill_checker_at(const int x, const int y) {
    Checker* checker = get_checker_at(x, y);
    checker->set_alive(false);
}

void Board::print_board() const {
    char text_board[NUM_ROWS][NUM_COLS] = {0};

    for(const Checker* checker : checkers) {
       if(checker->is_alive()) {
          text_board[checker->get_y()][checker->get_x()] =
              player_symbols[checker->get_player()];
       }
    }

    for(size_t i = 0; i < NUM_ROWS; i++) {
        for(size_t j = 0; j < NUM_COLS; j++) {
            if(!text_board[i][j]) {
                printf(". ");
            } else {
                printf("%c ", text_board[i][j]);
            }
        }
        printf("\n");
    }
}
/*
int Board::player_checker_count(const int playernum) const;
bool Board::can_player_move(const int playernum) const;
bool Board::is_move_legal(const int x_start, const int y_start,
        const int x_finish, const int y_finish) const;
*/
