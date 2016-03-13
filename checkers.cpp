#include "checkers.h"
#include <cstdio>
#include <vector>

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

Board::Board() {
    for(size_t i = 0; i < NUM_CHECKERS; i++) {
        Checker* checker0 = new Checker(0, player0_start_x[i],
                player0_start_y[i]);
        Checker* checker1 = new Checker(1, player1_start_x[i],
                player1_start_y[i]);
        checkers.insert(checker0);
        checkers.insert(checker1);
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
    checkers.erase(checker);
    delete checker;
}

void Board::print_board() const {
    char text_board[NUM_ROWS][NUM_COLS] = {0};

    for(const Checker* checker : checkers) {
        text_board[checker->get_y()][checker->get_x()] =
            player_symbols[checker->get_player()];
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

int Board::player_checker_count(const int playernum) const {
    int count = 0;
    for(const Checker* checker : checkers) {
        if(checker->get_player() == playernum) {
            count++;
        }
    }
    return count;
}

int Board::get_capture_vector(const Checker& checker) const {
    int x = checker.get_x();
    int y = checker.get_y();
    for(size_t i = 0; i < 2; i++) {
        int x_offset = diagonal_offsets[i][0];
        int y_offset = diagonal_offsets[i][1];

        Checker* victim = get_checker_at(x + x_offset,
                y + y_offset);
        
        if(not victim or victim->get_player() == checker.get_player()) {
            continue;
        }

        Checker* space = get_checker_at(victim->get_x() + x_offset,
                victim->get_y() + y_offset);
        
        if(not space) { //Empty space
            return i;
        }
    }
    return -1;
}

bool Board::is_move_capture(const Checker& checker, const int x_end, const int y_end) const {
    if(abs(x_end - checker.get_x()) != 2 or 
            abs(y_end - checker.get_x()) != 2) {
        return false;
    }
    
    if(get_checker_at(x_end, y_end)) {
        //there's something there
        return false;
    }

    int x_offset = x_end - checker.get_x(); //5 - 3 = 2
    int y_offset = y_end - checker.get_y();

    Checker* victim = get_checker_at(checker.get_x() + x_offset / 2,
            checker.get_y() + y_offset / 2);

    if(not victim or victim->get_player() == checker.get_player()) {
        return false;
    }
    return true;
}

bool Board::can_player_move(const int playernum) const {
    for(const Checker* checker : checkers) {
        if(checker->get_player() != playernum) {
            continue;
        }
        for(size_t i = 0; i < 2; i++) {
            int new_x = checker->get_x() + diagonal_offsets[i][0];
            int new_y = checker->get_y() + diagonal_offsets[i][1];
            if(not is_position_legal(new_x, new_y)) {
                continue;
            }
            if(not get_checker_at(new_x, new_y)) {
                return true;
            }
        }
    }
    return false;
}

bool Board::is_position_legal(const int x, const int y) const {
    return x >= 0 and x < NUM_COLS and y >= 0 and y < NUM_ROWS;
}

bool Board::is_move_legal(const Checker& piece, const int vector, const int player) const {
    return is_move_legal(piece.get_x(), piece.get_y(),
            piece.get_x() + diagonal_offsets[vector][0],
            piece.get_y() + diagonal_offsets[vector][1],
            player);
}

bool Board::is_move_legal(const int x_start, const int y_start,
        const int x_finish, const int y_finish, int player) const {
    if(not is_position_legal(x_finish, y_finish)) {
        return false;
    }

    Checker* checker = get_checker_at(x_start, y_start);
    if(not checker) {
        return false;
    }
    
    if(checker->get_player() != player) {
        return false;
    }

    if(is_move_capture(*checker, x_finish, y_finish)) {
        return true;
    }

    if(get_capture_vector(*checker) > -1) {
        //Could capture, so must capture
        return false;
    }

    //Can't capture, so as long as the move is one step away and empty
    // then it's OK
    if(abs(x_start - x_finish) != 1 or abs(y_start - y_finish) != 1) {
        return false;
    }

    if(get_checker_at(x_finish, y_finish)) {
        return false;
    }

    return true;
}

bool Board::can_player_move_piece(const Checker& checker, const int player) const {
    return can_player_move_piece(checker.get_x(), checker.get_y(), player);
}

bool Board::can_player_move_piece(const int x, const int y, const int player) const {
    if(not is_position_legal(x, y)) {
        return false;
    }
    Checker* checker = get_checker_at(x, y);
    if(not checker) {
        return false;
    }
    if(checker->get_player() != player) {
       return false;
    }
    
    for(size_t i = 0; i < 2; i++) {
        if(not get_checker_at(x + diagonal_offsets[i][0],
                    y + diagonal_offsets[i][1])) {
            return true;
        }
    }

    return false;
} 

void Board::make_capture(Checker& piece, const int vector) {
    int start_x = piece.get_x();
    int start_y = piece.get_y();
    int offset_x = diagonal_offsets[vector][0];
    int offset_y = diagonal_offsets[vector][1];
    kill_checker_at(start_x + offset_x, start_y + offset_y);
    piece.set_x(start_x + offset_x * 2); 
    piece.set_y(start_y + offset_y * 2);
}

void Board::make_random_move(const int player) {
    if(not can_player_move(player)) {
        return;
    }
    vector<Checker*> players_pieces;

    for(Checker* checker : checkers) {
        if(checker->get_player() == player) {
            players_pieces.push_back(checker);
        }
    }

    size_t piece_idx = rand() % players_pieces.size();

    while(not can_player_move_piece(*players_pieces[piece_idx], player)) {
        piece_idx = (piece_idx + 1) % players_pieces.size();
    }

    Checker* piece = players_pieces[piece_idx];
    //Now pick a move
    
    int capture_vector = get_capture_vector(*piece); 
    if(capture_vector > -1) {
        while(capture_vector > -1) {
            make_capture(*piece, capture_vector);
        }
    } else {
        int direction = rand() % 2;
        if(!is_move_legal(*piece, direction, player)) {
            direction = (direction + 1) % 2;
        } 
        piece->set_x(piece->get_x() + diagonal_offsets[direction][0]);
        piece->set_y(piece->get_y() + diagonal_offsets[direction][1]);
    }
}

Game::Game()
    :board()
{}

int Game::play() {
    int turn = 0;
    
    board.print_board();
    printf("========\n");
    
    while(board.can_player_move(0) and board.can_player_move(1)) {
        if(turn == 0) {
            get_player0_move();
        } else {
            get_player1_move();
        }
        turn = (turn + 1) % NUM_PLAYERS;
        
        printf("========\n");
        board.print_board();
        printf("========\n");
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

void Game::get_player1_move() {
    //Code to get player moves
    int start_x = -1, start_y = -1, end_x = -1, end_y = -1;
   
    printf("Human's turn\n");

    while(true) {
        do {
            do {
                printf("X position of piece to move: ");
                cin >> start_x;
                printf("Y position of piece to move: ");
                cin >> start_y;
            } while(board.can_player_move_piece(start_x, start_y, 1));

            do {
                printf("X position of destination: ");
                cin >> end_x;
                printf("Y position of destination: ");
                cin >> end_y;
            } while(not board.is_position_legal(end_x, end_y));
        } while(not board.is_move_legal(start_x, start_y, end_x, end_y, 1));
        
        Checker* checker = board.get_checker_at(start_x, start_y);
        
        //We can capture
        
        if(board.is_move_capture(*checker, end_x, end_y)) {
            board.kill_checker_at(start_x + (start_x - end_x),
                    start_y + (start_y - end_y));
            checker->set_x(end_x);
            checker->set_y(end_y);
            //If we can't capture again, the player's turn is over
            if(board.get_capture_vector(*checker) < 0) { 
                break;
            }
        }
        checker->set_x(end_x);
        checker->set_y(end_y);
    }
}


