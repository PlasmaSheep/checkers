#!/usr/bin/python

class Checkboard(object):
    def __init__(self):
        self.whites = set()
        self.blacks = set()
        self.checkers = {}
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
                if not ((col ^ row) & 1):
                    self.checkers[row][col] = piece
                    s.add((row, col))
        print self.whites
        print self.blacks

    def __str__(self):
        board = ''
        for row in range(7, -1, -1):
            for col in range(8):
                board += self.checkers[row].get(col, '.')
            board += '\n'
        return board


    def moved(self, row, col, nr, coldir, s):
        nc = col + coldir
        nnc = col + 2 * coldir
        if nc < 0 or nc > 7:
            return False  # this piece can not move

        if nc not in self.checkers[nr]:
            self.checkers[nr][nc] = self.checkers[row][col]
            del self.checkers[row][col]
            s.remove((row, col))
            s.add((nr, nc))
            return True
        return False


    def move(self, color):
        if color == 'w':
            s = self.whites
            dir = 1
        else:
            s = self.blacks
            dir = -1
        bs = set(s)
        for row, col in bs:
            nr = row + dir
            if row >= 0 and row < 8:
                for coldir in (1, -1):
                    if self.moved(row, col, nr, coldir, s):
                        return True
        return False


cb = Checkboard()
print cb
keepgoing = True
while keepgoing:
    for color in ('w', 'b'):
        if not cb.move(color):
            keepgoing = False
            break
        print cb

