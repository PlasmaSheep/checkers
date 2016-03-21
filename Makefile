CC = g++
CFLAGS = -g -O0 -Wall -Wextra -Werror -std=c++11
CHECKERS_SOURCES = checker.cpp \
				   game.cpp \
				   board.cpp \
				   playcheckers.cpp

all: checkers

checkers: $(CHECKERS_SOURCES)
	$(CC) $(CFLAGS) -o $@ $(CHECKERS_SOURCES)


clean:
	rm -rf checkers
