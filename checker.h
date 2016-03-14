#ifndef __CHECKER_H__
#define __CHECKER_H__

class Checker {
    private:
        int player;
        int x_pos;
        int y_pos;
    public:
        Checker(const int player, const int x_pos, const int y_pos);
        int get_player() const;
        void set_player(const int player);
        int get_x() const;
        int get_y() const;
        void set_x(const int);
        void set_y(const int);
        void set_alive(const bool alive);
};

#endif //__CHECKER_H__
