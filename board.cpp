#include "board.h"
#include "checker.h"
#include <vector>

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

int Board::y_for_player(int player) const {
    return player ? -1 : 1;
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

    printf("  ");
    for(size_t i = 0; i < NUM_COLS; i++) {
        printf("%zu ", i);
    }
    printf("\n");
    for(size_t i = 0; i < NUM_ROWS; i++) {
        printf("%zu ", i);
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
    int y_offset = y_for_player(checker.get_player());

    for(int i = 0;  i < 2; i++) {
	int x_offset = 1 - i * 2;

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
            abs(y_end - checker.get_y()) != 2) {
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
	int new_y = checker->get_y() + y_for_player(playernum);
        for(size_t i = 0; i < 2; i++) {
            int new_x = checker->get_x() + 1 - i * 2;
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
    int y_dir = y_for_player(player);
    int x_dir = vector ? 1 : -1;

    return is_move_legal(piece.get_x(), piece.get_y(),
			 piece.get_x() + x_dir,
			 piece.get_y() + y_dir,
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

    if(get_checker_at(x_finish, y_finish)) {
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

    int y_dir = y_for_player(player);

    for(size_t i = 0; i < 2; i++) {
	int x_dir = 1 - i * 2;

        if(not get_checker_at(x + x_dir,  y + y_dir)) {
            return true;
        }
	if(is_position_legal(x + x_dir * 2, y + y_dir * 2)) {
            Checker* space = get_checker_at(x + x_dir * 2,
					    y + y_dir * 2);
            Checker* victim = get_checker_at(x + x_dir, y + y_dir);
            if(victim and victim->get_player() != player and not space) {
                return true;
            }
       }
            //There is a checker there, can we cap it?

    }

    return false;
}

void Board::make_capture(Checker& piece, const int vector) {
    int start_x = piece.get_x();
    int start_y = piece.get_y();
    int player = piece.get_player();
    int offset_x = vector ? 1 : -1;
    int offset_y = y_for_player(player);
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
            capture_vector = get_capture_vector(*piece);
        }
    } else {
        int direction = rand() % 2;
        if(!is_move_legal(*piece, direction, player)) {
            direction = (direction + 1) % 2;
        }
        piece->set_x(piece->get_x() + (direction ? 1 : -1));
        piece->set_y(piece->get_y() + y_for_player(player));
    }
}
