#include "checker.h"

Checker::Checker(const int player, const int x_pos, const int y_pos)
   :player(player),
    x_pos(x_pos),
    y_pos(y_pos)
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
