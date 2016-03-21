#include "board.h"
#include "game.h"
#include <iostream>

using namespace std;

Game::Game()
    :board()
{}

int Game::play() {
    int turn = 0;

    board.print_board();

    while(board.can_player_move(0) and board.can_player_move(1)) {
        if(turn == 0) {
            get_player0_move();
        } else {
            get_player1_move();
        }
        turn = (turn + 1) % NUM_PLAYERS;

        board.print_board();
    }

    printf("Game over!");

    if(board.can_player_move(0)) {
        printf("Player 1 wins!\n");
        return 0;
    } else if(board.can_player_move(1)) {
        printf("Player 2 wins!\n");
        return 1;
    } else {
        printf("Draw!\n");
        return -1;
    }
}

void Game::get_player0_move() {
    //Code to run computer moves
    printf("Computer's turn\n");
    board.make_random_move(0);
}

void Game::input_next_move(int &start_x, int &start_y, int &end_x, int &end_y)
{
    string ui;
    cin >> ui;

    start_x = -1;

    if (ui.length() != 4) {
	return;
    }

    start_x = ui[0] - '0';
    start_y = ui[1] - '0';
    end_x = ui[2] - '0';
    end_y = ui[3] - '0';
}

void Game::get_player1_move() {
    //Code to get player moves
    int start_x = -1, start_y = -1, end_x = -1, end_y = -1;

    printf("Human's turn\n");

    while(true) {
	while (true) {
                printf("Enter your next move (4 symbols, from XY to XY): ");
		input_next_move(start_x, start_y, end_x, end_y);
		if (board.can_player_move_piece(start_x, start_y, 1) &&
		    board.is_position_legal(end_x, end_y) &&
		    board.is_move_legal(start_x, start_y, end_x, end_y, 1))
		    break;
		printf("this seems illegal\n");
	}

        Checker* checker = board.get_checker_at(start_x, start_y);

        //We can capture

        if(board.is_move_capture(*checker, end_x, end_y)) {
            board.kill_checker_at(start_x + (end_x - start_x) / 2,
                    start_y + (end_y - start_y)  / 2);
            checker->set_x(end_x);
            checker->set_y(end_y);
            //If we can't capture again, the player's turn is over
            if(board.get_capture_vector(*checker) < 0) {
                break;
            }
        }
        checker->set_x(end_x);
        checker->set_y(end_y);
        break;
    }
}
