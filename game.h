#ifndef __GAME_H__
#define __GAME_H__

#include "board.h"

class Game {
    private:
        Board board;
        void get_player0_move();
        void get_player1_move();
	void input_next_move(int &xs, int &ys, int &xe, int &ye);
    public:
        Game();
        int play();
};

#endif //__GAME_H__
