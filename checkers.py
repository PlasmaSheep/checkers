#!/usr/bin/python

import sys

class Checkboard(object):
    def __init__(self):
        self.whites = set()  # All white pieces, tuples of coordinates
        self.blacks = set()  # All black pieces, tuples of coordinates
        self.checkers = {}   # The entire board, only taken spots, dictionary of dictionaries [row][col]

        # Populate the board
        for row in range(8):
            self.checkers[row] = {}
            for col in range(8):
                if row < 3:
                    piece = 'w'
                    s = self.whites
                elif row > 4:
                    piece = 'b'
                    s = self.blacks
                else:
                    continue
                if not ((col ^ row) & 1):  # Only every other cell gets a piece
                    self.checkers[row][col] = piece
                    s.add((row, col))

    def __str__(self):
        separator = ' ' + '+-' * 8 + '+\n'
        board = separator
        for row in range(7, -1, -1):
            board += str(row)
            for col in range(8):
                if (row ^ col) & 1:
                    check = ' '
                else:
                    check = '.'
                board += '|' + self.checkers[row].get(col, check)
            board += '|\n' + separator
        board += ' '
        for col in range(8):
            board += ' ' + str(col)
        return board

    # Return true if the piece in [row][col] was able to capture an opponent piece
    def captured(self, row, col, rowdir, coldir, mycolor):
        nnc = col + 2 * coldir
        nnr = row + 2 * rowdir
        if nnc < 0 or nnc > 7 or nnr < 0 or nnr > 7:
            return False  # this piece can not capture

        nc = col + coldir
        nr = row + rowdir

        if nnc in self.checkers[nnr] or not nc in self.checkers[nr] or self.checkers[nr][nc] == mycolor:
            return False  # there is nothing to capture

        # Determine the this and other sets
        if mycolor == 'w':
            os = self.blacks
            s = self.whites
        else:
            os = self.whites
            s = self.blacks

        # remove the captured piece
        del self.checkers[nr][nc]
        os.remove((nr, nc))

        # move our piece
        self.checkers[nnr][nnc] = mycolor
        del self.checkers[row][col]
        s.remove((row, col))
        s.add((nnr, nnc))
        return True

    # return true if piece in [row][col] was able to move
    def moved(self, row, col, rowdir, coldir, mycolor):
        nr = row + rowdir
        nc = col + coldir

        if nr < 0 or nr > 7 or nc < 0 or nc > 7:
            return False  # no chance to move

        if nc not in self.checkers[nr]:
            del self.checkers[row][col]
            self.checkers[nr][nc] = mycolor
            if mycolor == 'w':
                s = self.whites
            else:
                s = self.blacks
            s.remove((row, col))
            s.add((nr, nc))
            return True
        return False


    def try_capture(self, row, col, rowdir, color):
        for coldir in (1, -1):
            if self.captured(row, col, rowdir, coldir, color):
                return True
        return False

    def try_move(self, row, col, rowdir, color):
        for coldir in (1, -1):
            if self.moved(row, col, rowdir, coldir, color):
                return True
        return False

    def move(self, color):
        # create a copy set, so that we can iterate over it and modify the original set
        if color == 'w':
            cs = set(self.whites)
            rowdir = 1
        else:
            cs = set(self.blacks)
            rowdir = -1

        # Captures when possible must proceed, try them first
        for row, col in cs:
            if self.try_capture(row, col, rowdir, color):
                return True
        for row, col in cs:
            if self.try_move(row, col, rowdir, color):
                    return True
        return False


def get_coord(cb):
    while True:
        sys.stdout.write('Your move: ')
        sys.stdout.flush()
        try:
            line = sys.stdin.readline().strip()
        except KeyboardInterrupt:
            print
            sys.exit(1)
        if len(line) == 3:
            row, col, dir = (int(x) for x in line)
            if cb.checkers[row].get(col, '') == 'b' and dir < 2:
                return row, col, dir * 2 - 1
        sys.stdout.write('Invalid input, try again\n')

cb = Checkboard()
keepgoing = True
while keepgoing:
    if True:  # Computer vs human
        if not cb.move('w'):
            break
        print cb
        row, col, dir = get_coord(cb)
        if cb.captured(row, col, -1, dir, 'b') or cb.moved(row, col, -1, dir, 'b'):
            continue
        break
    for color in ('w', 'b'):
        print '\nbefore %s move :' % color
        print cb
        if not cb.move(color):
            keepgoing = False
            break
