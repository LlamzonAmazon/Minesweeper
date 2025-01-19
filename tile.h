#ifndef TILE_H
#define TILE_H

#include <QPushButton>

struct tile {
    QPushButton *button;
    bool isMine;
    int adjMines;
    int row;
    int col;

    tile(QPushButton *b, bool m, int a, int r, int c)
        : button(b), isMine(m), adjMines(a), row(r), col(c) {}
};

#endif // TILE_H
