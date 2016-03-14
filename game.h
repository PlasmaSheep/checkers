#ifndef __GAME_H__
#define __GAME_H__

#include "board.h"

class Game {
    private:
        Board board;
        void get_player0_move();
        void get_player1_move();
    public:
        Game();
        int play();
};

#endif //__GAME_H__
